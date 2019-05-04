#ifndef RESOURCES_PARSER_H
#define RESOURCES_PARSER_H

#include "ResourceTypes.h"

#include <string>
#include <map>
#include <vector>
#include <fstream>

class ResourcesParser {
public:
	struct StringInfoIndex {
		StringInfoIndex(uint32_t keyIndex, uint32_t valueIndex) {
			this->keyIndex = keyIndex;
			this->valueIndex = valueIndex;
		}
		uint32_t keyIndex;
		uint32_t valueIndex;
	};

	typedef std::vector<StringInfoIndex>
		StringInfoIndexs;

	typedef std::map<std::string, StringInfoIndexs>
		StringInfoIndexInConfigs;

	typedef std::map<std::string, StringInfoIndexInConfigs>
		StringInfoIndexInPackage;

	typedef std::map<std::string, std::vector<std::string> >
		StringKeyInPackage;

public:
	ResourcesParser(const std::string& filePath);

	const std::vector<std::string>& getGlobalStringPool() {
		return mGlobalStringPool;
	}

	const StringInfoIndexInPackage& getStringInfoIndexInPackage() {
		return mStringInfoIndexInPackage;
	}

	const StringKeyInPackage& getStringKeyInPackage() {
		return mStringKeyInPackage;
	}
private:
	ResTable_header mResourcesInfo;
	std::vector<ResTable_package> mPackageTables;
	std::vector<std::string> mGlobalStringPool;
	StringKeyInPackage mStringKeyInPackage;
	StringInfoIndexInPackage mStringInfoIndexInPackage;

	bool parserStringPool(
			std::ifstream& resources,
			std::vector<std::string>* pStringPool);

	bool parserPackageInfo(std::ifstream& resources);

	bool parseStringFromPackage(
			std::ifstream& resources,
			const std::vector<std::string>& keys,
			int stringId,
			const std::string& packageName);

	bool parseStringFromResTable(
			std::ifstream& resources,
			const std::vector<std::string>& keys,
			const ResTable_type& header,
			StringInfoIndexInConfigs* pStringInfoIndexInConfigs);

};

#endif  /*RESOURCES_PARSER_H*/
