#include "ResourcesParser.h"

#include <codecvt>
#include <locale>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>

#define RETURN_UNKNOWN_ID(ID) stringstream ss; \
	ss <<"???\(0x" <<hex <<ID <<")"; \
	return ss.str()

using namespace std;

static string stringOfComplex(uint32_t complex, bool isFraction) {
	stringstream ss;
    const float MANTISSA_MULT =
        1.0f / (1<<Res_value::COMPLEX_MANTISSA_SHIFT);
    const float RADIX_MULTS[] = {
        1.0f*MANTISSA_MULT, 1.0f/(1<<7)*MANTISSA_MULT,
        1.0f/(1<<15)*MANTISSA_MULT, 1.0f/(1<<23)*MANTISSA_MULT
    };

    float value = (complex&(Res_value::COMPLEX_MANTISSA_MASK
                   <<Res_value::COMPLEX_MANTISSA_SHIFT))
            * RADIX_MULTS[(complex>>Res_value::COMPLEX_RADIX_SHIFT)
                            & Res_value::COMPLEX_RADIX_MASK];
	ss<<value;

    if (!isFraction) {
        switch ((complex>>Res_value::COMPLEX_UNIT_SHIFT)&Res_value::COMPLEX_UNIT_MASK) {
            case Res_value::COMPLEX_UNIT_PX: ss<<"px"; break;
            case Res_value::COMPLEX_UNIT_DIP: ss<<"dp"; break;
            case Res_value::COMPLEX_UNIT_SP: ss<<"sp"; break;
            case Res_value::COMPLEX_UNIT_PT: ss<<"pt"; break;
            case Res_value::COMPLEX_UNIT_IN: ss<<"in"; break;
            case Res_value::COMPLEX_UNIT_MM: ss<<"mm"; break;
            default: ss<<" (unknown unit)"; break;
        }
    } else {
        switch ((complex>>Res_value::COMPLEX_UNIT_SHIFT)&Res_value::COMPLEX_UNIT_MASK) {
            case Res_value::COMPLEX_UNIT_FRACTION: ss<<"%"; break;
            case Res_value::COMPLEX_UNIT_FRACTION_PARENT: ss<<"%p"; break;
            default: ss<<" (unknown unit)"; break;
        }
    }
	return ss.str();
}

string ResourcesParser::stringOfValue(const Res_value* value) const {
	stringstream ss;
    if (value->dataType == Res_value::TYPE_NULL) {
        ss<<"(null)";
    } else if (value->dataType == Res_value::TYPE_REFERENCE) {
		ss<<"(reference) "<<getNameForId(value->data);
    } else if (value->dataType == Res_value::TYPE_ATTRIBUTE) {
		ss<<"(attribute) "<<getNameForId(value->data);
    } else if (value->dataType == Res_value::TYPE_STRING) {
		ss<<"(string) "<<getStringFromGlobalStringPool(value->data);
    } else if (value->dataType == Res_value::TYPE_FLOAT) {
        ss<<"(float) "<<*(const float*)&value->data;
    } else if (value->dataType == Res_value::TYPE_DIMENSION) {
        ss<<"(dimension) "<<stringOfComplex(value->data, false);
    } else if (value->dataType == Res_value::TYPE_FRACTION) {
        ss<<"(fraction) "<<stringOfComplex(value->data, true);
    } else if (value->dataType >= Res_value::TYPE_FIRST_COLOR_INT
            && value->dataType <= Res_value::TYPE_LAST_COLOR_INT) {
		ss<<"(color) #"<<hex<<setw(8)<<setfill('0')<<value->data;
    } else if (value->dataType == Res_value::TYPE_INT_BOOLEAN) {
        ss<<"(boolean) "<<(value->data ? "true" : "false");
    } else if (value->dataType >= Res_value::TYPE_FIRST_INT
            && value->dataType <= Res_value::TYPE_LAST_INT) {
        ss<<"(int) "<<value->data<<" or 0x"<<hex<<setw(8)<<setfill('0')<<value->data;
    } else {
		ss<<"(unknown type) "
			<<"t=0x"<<hex<<setw(2)<<setfill('0')<<(int)value->dataType<<" "
			<<"d=0x"<<hex<<setw(8)<<setfill('0')<<(int)value->data<<" "
			<<"(s=0x"<<hex<<setw(4)<<setfill('0')<<(int)value->size<<" "
			<<"r=0x"<<hex<<setw(2)<<setfill('0')<<(int)value->res0<<")";
    }
	return ss.str();
}

inline static string toUtf8(const u16string& str16) {
	return wstring_convert<codecvt_utf8_utf16<char16_t>,char16_t>()
		.to_bytes(str16);
}

ResourcesParser::ResourcesParser(const string& filePath) {
	ifstream resources(filePath, ios::in|ios::binary);

	// resources文件开头是个ResTable_header,记录整个文件的信息
	resources.read((char*)&mResourcesInfo, sizeof(ResTable_header));

	// 紧接着就是全局字符串池
	mGlobalStringPool = parserResStringPool(resources);
	if(mGlobalStringPool == nullptr) {
		return;
	}

	for(int i = 0 ; i < mResourcesInfo.packageCount ; i++) {
		PackageResourcePtr pResource = parserPackageResource(resources);
		if(pResource == nullptr) {
			return;
		}
		mResourceForId[pResource->header.id] = pResource;
		mResourceForPackageName[toUtf8((char16_t*)pResource->header.name)] = pResource;
	}
}

ResourcesParser::ResStringPoolPtr ResourcesParser::parserResStringPool(
		ifstream& resources) {
	ResStringPoolPtr pPool = make_shared<ResStringPool>();
	resources.read((char*)&pPool->header, sizeof(ResStringPool_header));
	if(pPool->header.header.type != RES_STRING_POOL_TYPE) {
		cout<<"parserResStringPool 需要定位到 RES_STRING_POOL_TYPE !"<<endl;
		return nullptr;
	}

	pPool->pOffsets = shared_ptr<uint32_t>(
			new uint32_t[pPool->header.stringCount],
			default_delete<uint32_t[]>()
	);

	const uint32_t offsetSize = sizeof(uint32_t) * pPool->header.stringCount;
	resources.read((char*)pPool->pOffsets.get(), offsetSize);

	// 跳到字符串数组开头位置
	uint32_t seek = pPool->header.stringsStart
		- pPool->header.header.headerSize
		- offsetSize;
	resources.seekg(seek, ios::cur);

	// 载入所有字符串
	const uint32_t strBuffSize = pPool->header.styleCount > 0
		? pPool->header.stylesStart - pPool->header.stringsStart
		: pPool->header.header.size - pPool->header.stringsStart;
	pPool->pStrings = shared_ptr<byte>(
			new byte[strBuffSize],
			default_delete<byte[]>()
	);
	resources.read((char*)pPool->pStrings.get(), strBuffSize);

	// 跳出全局字符串池
	if(pPool->header.styleCount > 0) {
		seek = pPool->header.header.size
			- pPool->header.stringsStart
			- strBuffSize;
		resources.seekg(seek, ios::cur);
	}

	return pPool;
}

string ResourcesParser::getStringFromGlobalStringPool(uint32_t index) const {
	return getStringFromResStringPool(mGlobalStringPool, index);
}

string ResourcesParser::getStringFromResStringPool(
			ResourcesParser::ResStringPoolPtr pPool,
			uint32_t index) {
	if(index > pPool->header.stringCount) {
		return "???";
	}
	uint32_t offset = *(pPool->pOffsets.get() + index);
	//前两个字节是字符串长度
	char* str = ((char*) (pPool->pStrings.get() + offset + 2));
	return (pPool->header.flags & ResStringPool_header::UTF8_FLAG)
		? str
		: toUtf8((char16_t*) str);
}

ResourcesParser::PackageResourcePtr ResourcesParser::parserPackageResource(
		ifstream& resources) {
	PackageResourcePtr pPool = make_shared<PackageResource>();
	resources.read((char*)&pPool->header, sizeof(ResTable_package));

	if(pPool->header.header.type != RES_TABLE_PACKAGE_TYPE) {
		cout<<"parserPackageResource 需要定位到 RES_TABLE_PACKAGE_TYPE !"<<endl;
		return nullptr;
	}

	// 接着是资源类型字符串池
	pPool->pTypes = parserResStringPool(resources);

	// 接着是资源名称字符串池
	pPool->pKeys = parserResStringPool(resources);

	ResChunk_header chunkHeader;
	while(resources.read((char*)&chunkHeader, sizeof(ResChunk_header))) {
		resources.seekg(-sizeof(ResChunk_header), ios::cur);

		if(chunkHeader.type == RES_TABLE_PACKAGE_TYPE) {
			return pPool;
		} else if(chunkHeader.type == RES_TABLE_TYPE_TYPE) {
			ResTableTypePtr pResTableType = make_shared<ResTableType>();
			resources.read((char*)&pResTableType->header, sizeof(ResTable_type));
			uint32_t seek = pResTableType->header.header.headerSize - sizeof(ResTable_type);
			resources.seekg(seek, ios::cur);
			pResTableType->entryPool = parserEntryPool(
					resources,
					pResTableType->header.entryCount,
					pResTableType->header.entriesStart - pResTableType->header.header.headerSize,
					pResTableType->header.header.size - pResTableType->header.entriesStart);
			pPool->resTablePtrs[pResTableType->header.id].push_back(pResTableType);
			for(int i = 0 ; i < pResTableType->header.entryCount ; i++) {
				ResTable_entry* pEntry = getEntryFromEntryPool(pResTableType->entryPool, i);
				if(nullptr == pEntry) {
					pResTableType->entries.push_back(nullptr);
					pResTableType->values.push_back(nullptr);
					continue;
				}
				Res_value* pValue = getValueFromEntry(pEntry);
				pResTableType->entries.push_back(pEntry);
				pResTableType->values.push_back(pValue);
			}
		} else {
			resources.seekg(chunkHeader.size, ios::cur);
		}
	}

	return pPool;
}

ResourcesParser::EntryPool ResourcesParser::parserEntryPool(
			ifstream& resources,
			uint32_t entryCount,
			uint32_t dataStart,
			uint32_t dataSize) {
	EntryPool pool;
	pool.pOffsets = shared_ptr<uint32_t>(
			new uint32_t[entryCount],
			default_delete<uint32_t[]>()
	);

	const uint32_t offsetSize = sizeof(uint32_t) * entryCount;
	resources.read((char*)pool.pOffsets.get(), offsetSize);

	pool.offsetCount = entryCount;
	pool.dataSize = dataSize;

	resources.seekg(dataStart - offsetSize, ios::cur);

	pool.pData = shared_ptr<byte>(
			new byte[pool.dataSize],
			default_delete<byte[]>()
	);
	resources.read((char*)pool.pData.get(), pool.dataSize);
	return pool;
}

ResTable_entry* ResourcesParser::getEntryFromEntryPool(EntryPool pool, uint32_t index) {
	if(index >= pool.offsetCount) {
		return nullptr;
	}
	uint32_t offset = *(pool.pOffsets.get() + index);
	if(offset == ResTable_type::NO_ENTRY) {
		return nullptr;
	}
	return ((ResTable_entry*)(pool.pData.get() + offset));
}

Res_value* ResourcesParser::getValueFromEntry(const ResTable_entry* pEntry) {
	return (Res_value*)(((byte*)pEntry) + pEntry->size);
}

ResTable_map* ResourcesParser::getMapsFromEntry(const ResTable_entry* pEntry) {
	return (ResTable_map*)(((byte*)pEntry) + pEntry->size);
}


ResourcesParser::PackageResourcePtr ResourcesParser::getPackageResouceForId(uint32_t id) const {
	uint32_t packageId = (id >> 24);
	auto it = mResourceForId.find(packageId);
	if(it==mResourceForId.end()) {
		return nullptr;
	}
	return it->second;
}

vector<ResourcesParser::ResTableTypePtr> ResourcesParser::getResTableTypesForId(uint32_t id) {
	PackageResourcePtr pPackage = getPackageResouceForId(id);
	if(pPackage == nullptr) {
		return vector<ResTableTypePtr>();
	}
	uint32_t typeId = TYPE_ID(id);
	if(pPackage->resTablePtrs.size()<=typeId){
		return vector<ResTableTypePtr>();
	}

	return pPackage->resTablePtrs[typeId];
}

string ResourcesParser::getNameForId(uint32_t id) const {
	PackageResourcePtr pPackage = getPackageResouceForId(id);
	if(pPackage == nullptr) {
		RETURN_UNKNOWN_ID(id);
	}
	uint32_t typeId = TYPE_ID(id);
	if(pPackage->resTablePtrs.size()<=typeId){
		RETURN_UNKNOWN_ID(id);
	}

	uint32_t entryId = ENTRY_ID(id);

	if(pPackage->resTablePtrs[typeId][0]->header.entryCount<=(entryId)){
		RETURN_UNKNOWN_ID(id);
	}

	const ResTable_entry* pEntry = nullptr;
	for(ResTableTypePtr  pResTableType : pPackage->resTablePtrs[typeId]) {
		pEntry = pResTableType->entries[entryId];
		if(pEntry) {
			break;
		}
	}

	if(pEntry == nullptr) {
		RETURN_UNKNOWN_ID(id);
	}
	return getStringFromResStringPool(getPackageResouceForId(id)->pKeys, pEntry->key.index);
}

string ResourcesParser::getNameForResTableMap(const ResTable_ref& ref) const {
	switch(ref.ident) {
		case ResTable_map::ATTR_TYPE:
			return "ATTR_TYPE";
        case ResTable_map::ATTR_MIN:
			return "ATTR_MIN";
		case ResTable_map::ATTR_MAX:
			return "ATTR_MAX";
		case ResTable_map::ATTR_L10N:
			return "ATTR_L10N";
       	case ResTable_map::ATTR_OTHER:
			return "ATTR_OTHER";
       	case ResTable_map::ATTR_ZERO:
			return "ATTR_ZERO";
       	case ResTable_map::ATTR_ONE:
			return "ATTR_ONE";
       	case ResTable_map::ATTR_TWO:
			return "ATTR_TWO";
       	case ResTable_map::ATTR_FEW:
			return "ATTR_FEW";
       	case ResTable_map::ATTR_MANY:
			return "ATTR_MANY";
		default:
			return getNameForId(ref.ident);
	}
}

bool ResourcesParser::isTableMapForAttrDesc(const ResTable_ref& ref) {
	switch(ref.ident) {
		case ResTable_map::ATTR_TYPE:
        case ResTable_map::ATTR_MIN:
		case ResTable_map::ATTR_MAX:
		case ResTable_map::ATTR_L10N:
       	case ResTable_map::ATTR_OTHER:
       	case ResTable_map::ATTR_ZERO:
       	case ResTable_map::ATTR_ONE:
       	case ResTable_map::ATTR_TWO:
       	case ResTable_map::ATTR_FEW:
       	case ResTable_map::ATTR_MANY:
			return true;
		default:
			return false;
	}
}

string ResourcesParser::getValueTypeForResTableMap(const Res_value& value) const {
	switch(value.data) {
		case ResTable_map::TYPE_ANY:
			return "any";
		case ResTable_map::TYPE_REFERENCE:
			return "reference";
		case ResTable_map::TYPE_STRING:
			return "string";
		case ResTable_map::TYPE_INTEGER:
			return "integer";
		case ResTable_map::TYPE_BOOLEAN:
			return "boolean";
		case ResTable_map::TYPE_COLOR:
			return "color";
		case ResTable_map::TYPE_FLOAT:
			return "float";
		case ResTable_map::TYPE_DIMENSION:
			return "dimension";
		case ResTable_map::TYPE_FRACTION:
			return "fraction";
		case ResTable_map::TYPE_ENUM:
			return "enum";
		case ResTable_map::TYPE_FLAGS:
			return "flags";
		default:
			return "unknown";
	}
}
