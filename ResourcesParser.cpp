#include "ResourcesParser.h"

#include <codecvt>
#include <locale>
#include <algorithm>
#include <sstream>
#include <iomanip>

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
            default: printf(" (unknown unit)"); break;
        }
    } else {
        switch ((complex>>Res_value::COMPLEX_UNIT_SHIFT)&Res_value::COMPLEX_UNIT_MASK) {
            case Res_value::COMPLEX_UNIT_FRACTION: ss<<"%"; break;
            case Res_value::COMPLEX_UNIT_FRACTION_PARENT: ss<<"%p"; break;
            default: printf(" (unknown unit)"); break;
        }
    }
	return ss.str();
}

string ResourcesParser::stringOfValue(const Res_value* value) {
	stringstream ss;
    if (value->dataType == Res_value::TYPE_NULL) {
        ss<<"(null)";
    } else if (value->dataType == Res_value::TYPE_REFERENCE) {
		ss<<"(reference) 0x"<<hex<<setw(8)<<setfill('0')<<value->data;
    } else if (value->dataType == Res_value::TYPE_ATTRIBUTE) {
		ss<<"(attribute) 0x"<<hex<<setw(8)<<setfill('0')<<value->data;
    } else if (value->dataType == Res_value::TYPE_STRING) {
		ss<<"(string) "<<getStringFromGlobalStringPool(value->data);
    } else if (value->dataType == Res_value::TYPE_FLOAT) {
        ss<<"(float) "<<*(const float*)&value->data;
    } else if (value->dataType == Res_value::TYPE_DIMENSION) {
        ss<<"(dimension) "<<stringOfComplex(value->data, false);
    } else if (value->dataType == Res_value::TYPE_FRACTION) {
        ss<<"(fraction) "<<stringOfComplex(value->data, true);
    } else if (value->dataType >= Res_value::TYPE_FIRST_COLOR_INT
            || value->dataType <= Res_value::TYPE_LAST_COLOR_INT) {
		ss<<"(color) #"<<hex<<setw(8)<<setfill('0')<<value->data;
    } else if (value->dataType == Res_value::TYPE_INT_BOOLEAN) {
        ss<<"(boolean) "<<(value->data ? "true" : "false");
    } else if (value->dataType >= Res_value::TYPE_FIRST_INT
            || value->dataType <= Res_value::TYPE_LAST_INT) {
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
		printf("parserResStringPool 需要定位到 RES_STRING_POOL_TYPE !\n");
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

string ResourcesParser::getStringFromGlobalStringPool(uint32_t index) {
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
	return ((char*) (pPool->pStrings.get() + offset + 2));
}

ResourcesParser::PackageResourcePtr ResourcesParser::parserPackageResource(
		ifstream& resources) {
	PackageResourcePtr pPool = make_shared<PackageResource>();
	resources.read((char*)&pPool->header, sizeof(ResTable_package));

	if(pPool->header.header.type != RES_TABLE_PACKAGE_TYPE) {
		printf("parserPackageResource 需要定位到 RES_TABLE_PACKAGE_TYPE !\n");
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
			resources.seekg(pResTableType->header.header.headerSize -sizeof(ResTable_type) , ios::cur);
			pResTableType->entryPool = parserEntryPool(
					resources,
					pResTableType->header.entryCount,
					pResTableType->header.entriesStart - pResTableType->header.header.headerSize);
			uint32_t seek = pResTableType->header.header.size
				- pResTableType->header.entriesStart - pResTableType->entryPool.dataCount;
			resources.seekg(seek, ios::cur);
			pPool->resTablePtrs[pResTableType->header.id].push_back(pResTableType);
			for(int i = 0 ; i < pResTableType->header.entryCount ; i++) {
				const ResTable_entry* pEntry = getEntryFromEntryPool(pResTableType->entryPool, i);
				if(nullptr == pEntry) {
					pResTableType->entries.push_back(nullptr);
					pResTableType->values.push_back(nullptr);
					continue;
				}
				const Res_value* pValue = getValueFromEntry(pEntry);
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
			uint32_t dataCount,
			uint32_t dataStart) {
	EntryPool pool;
	pool.pOffsets = shared_ptr<uint32_t>(
			new uint32_t[dataCount],
			default_delete<uint32_t[]>()
	);

	const uint32_t offsetSize = sizeof(uint32_t) * dataCount;
	resources.read((char*)pool.pOffsets.get(), offsetSize);

	pool.offsetCount = dataCount;
	pool.dataCount = getEntryPoolDataBuffSize(pool.pOffsets.get(), dataCount);

	resources.seekg(dataStart - offsetSize, ios::cur);

	pool.pData = shared_ptr<byte>(
			new byte[pool.dataCount],
			default_delete<byte[]>()
	);
	resources.read((char*)pool.pData.get(), pool.dataCount);
	return pool;
}

uint32_t ResourcesParser::getEntryPoolDataBuffSize(
		const uint32_t* pData,
		uint32_t entryCount) {
	uint32_t maximumOffset = ResTable_type::NO_ENTRY;
	int offset;
	for(int i = 0 ; i < entryCount ; i++) {
		offset = pData[i];
		if(offset != ResTable_type::NO_ENTRY
				&& (maximumOffset == ResTable_type::NO_ENTRY || maximumOffset < offset)) {
				maximumOffset = offset;
		}
	}

	return maximumOffset != ResTable_type::NO_ENTRY
			? maximumOffset + (sizeof(ResTable_entry) + sizeof(Res_value))
			: 0;
}

const ResTable_entry* ResourcesParser::getEntryFromEntryPool(EntryPool pool, uint32_t index) {
	if(index >= pool.offsetCount) {
		return nullptr;
	}
	uint32_t offset = *(pool.pOffsets.get() + index);
	if(offset == ResTable_type::NO_ENTRY) {
		return nullptr;
	}
	return ((ResTable_entry*)(pool.pData.get() + offset));
}

const Res_value* ResourcesParser::getValueFromEntry(const ResTable_entry* pEntry) {
	return (Res_value*)(((byte*)pEntry) + pEntry->size);
}
