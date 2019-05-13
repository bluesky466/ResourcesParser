#ifndef RESOURCES_PARSER_H
#define RESOURCES_PARSER_H

#include "ResourceTypes.h"

#include <string>
#include <list>
#include <map>
#include <vector>
#include <fstream>
#include <memory>

#define TYPE_ID(X) ((X & 0x00FF0000) >> 16)
#define ENTRY_ID(X) (X & 0xFFFF)

class ResourcesParser {
public:
	typedef unsigned char byte;

	struct EntryPool {
		std::shared_ptr<uint32_t> pOffsets;
		std::shared_ptr<byte> pData;
		uint32_t dataSize;
		uint32_t offsetCount;
	};

	struct ResStringPool {
		ResStringPool_header header;
		std::shared_ptr<uint32_t> pOffsets;
		std::shared_ptr<byte> pStrings;
	};
	typedef std::shared_ptr<ResStringPool> ResStringPoolPtr;

	struct ResTableType {
		ResTable_type header;
		EntryPool entryPool;
		std::vector<ResTable_entry*> entries;
		std::vector<Res_value*> values;
		std::vector<std::vector<ResTable_map*> > maps;
	};
	typedef std::shared_ptr<ResTableType> ResTableTypePtr;

	struct PackageResource {
		ResTable_package header;
		ResStringPoolPtr pTypes;
		ResStringPoolPtr pKeys;
		std::map<int, std::vector<ResTableTypePtr> > resTablePtrs;
	};
	typedef std::shared_ptr<PackageResource> PackageResourcePtr;

public:
	ResourcesParser(const std::string& filePath);

	static std::string getStringFromResStringPool(ResStringPoolPtr pPool, uint32_t index);

	static bool isTableMapForAttrDesc(const ResTable_ref& ref);

	const std::map<std::string, PackageResourcePtr>& getResourceForPackageName() const {
		return mResourceForPackageName;
	}
	std::string getStringFromGlobalStringPool(uint32_t index) const;

	PackageResourcePtr getPackageResouceForId(uint32_t id) const;

	std::vector<ResTableTypePtr> getResTableTypesForId(uint32_t id);

	std::string getNameForId(uint32_t id) const;

	std::string getNameForResTableMap(const ResTable_ref& ref) const;

	std::string getValueForResTableMap(const Res_value& value) const;

	std::string stringOfValue(const Res_value* value) const;

private:
	ResTable_header mResourcesInfo;
	ResStringPoolPtr mGlobalStringPool;

	std::map<std::string, PackageResourcePtr> mResourceForPackageName;
	std::map<uint32_t, PackageResourcePtr> mResourceForId;
	std::vector<ResTable_package> mPackageTables;

	ResStringPoolPtr parserResStringPool(std::ifstream& resources);

	PackageResourcePtr parserPackageResource(std::ifstream& resources);

	EntryPool parserEntryPool(
			std::ifstream& resources,
			uint32_t entryCount,
			uint32_t dataStart,
			uint32_t dataSize);

	ResTable_entry* getEntryFromEntryPool(EntryPool pool, uint32_t index);

	Res_value* getValueFromEntry(const ResTable_entry* pEntry);

	ResTable_map* getMapsFromEntry(const ResTable_entry* pEntry);
};

#endif  /*RESOURCES_PARSER_H*/
