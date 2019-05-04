#include "ResourcesParser.h"

#include <codecvt>

using namespace std;

inline static string toUtf8(const u16string& str16) {
	return wstring_convert<codecvt_utf8_utf16<char16_t>,char16_t>()
		.to_bytes(str16);
}

ResourcesParser::ResourcesParser(const std::string& filePath) {
	ifstream resources(filePath, ios::in|ios::binary);

	// resources文件开头是个ResTable_header,记录整个文件的信息
	resources.read((char*)&mResourcesInfo, sizeof(ResTable_header));

	// 紧接着就是全局字符串池
	if(!parserStringPool(resources, &mGlobalStringPool)) {
		return;
	}

	for(int i = 0 ; i < mResourcesInfo.packageCount ; i++) {
		if(!parserPackageInfo(resources)) {
			return;
		}
	}
}

bool ResourcesParser::parserStringPool(
		ifstream& resources,
		vector<string>* pStringPool) {
	ResStringPool_header header;
	resources.read((char*)&header, sizeof(ResStringPool_header));
	if(header.header.type != RES_STRING_POOL_TYPE) {
		printf("parserStringPool需要定位到RES_STRING_POOL_TYPE!\n");
		return false;
	}

	// ResStringPool_header后面紧接着就是字符串偏移数组
	const uint32_t stringCount = header.stringCount;
	const uint32_t* offsets = new uint32_t[stringCount];
	const uint32_t offsetSize = sizeof(uint32_t) * stringCount;
	resources.read((char*)offsets, offsetSize);

	// 跳到字符串数组开头位置
	resources.seekg(
		header.stringsStart - header.header.headerSize - offsetSize,
		ios::cur
	);

	// 载入所有字符串
	const uint32_t strBuffSize = header.styleCount > 0
		? header.stylesStart - header.stringsStart
		: header.header.size - header.stringsStart;
	char* strBuff = new char[strBuffSize];
	resources.read(strBuff, strBuffSize);

	for(int i = 0 ; i < header.stringCount ; i++) {
		// 前两个字节是字符串的长度
		if(header.flags != ResStringPool_header::UTF8_FLAG) {
			u16string str16 = u16string((char16_t*)(strBuff + offsets[i] + 2));
			pStringPool->push_back(toUtf8(str16));
		} else {
			pStringPool->push_back(strBuff + offsets[i] + 2);
		}
	}

	delete[] offsets;
	delete[] strBuff;

	// 跳出全局字符串池
	if(header.styleCount > 0) {
		resources.seekg(
			header.header.size - header.stringsStart - strBuffSize,
			ios::cur
		);
	}
	return true;
}

bool ResourcesParser::parserPackageInfo(ifstream& resources) {
	ResTable_package header;
	resources.read((char*)&header, sizeof(ResTable_package));

	if(header.header.type != RES_TABLE_PACKAGE_TYPE) {
		printf("parserPackageInfo需要定位到RES_TABLE_PACKAGE_TYPE!\n");
		return false;
	}

	mPackageTables.push_back(header);

	string packageName = toUtf8((char16_t*)header.name);

	// 接着是资源类型字符串池
	vector<string> types;
	parserStringPool(resources, &types);

	// 接着是资源名称字符串池
	vector<string> keys;
	parserStringPool(resources, &keys);
	mStringKeyInPackage[packageName] = keys;

	int stringId
	   	= distance(types.begin(), find(types.begin(), types.end(), "string"))
		+ 1;
	return parseStringFromPackage(
			resources,
			keys,
			stringId,
			packageName
	);
}

bool ResourcesParser::parseStringFromResTable(
		ifstream& resources,
		const vector<string>& keys,
		const ResTable_type& header,
		StringInfoIndexInConfigs* pStringInfoIndexInConfigs) {
	uint32_t* pOffsets = new uint32_t[header.entryCount];
	streamsize offsesSize = sizeof(uint32_t) * header.entryCount;
	resources.read(
			(char*)pOffsets,
			offsesSize
	);

	uint16_t headerSize = header.header.headerSize;
	resources.seekg(
			header.entriesStart - headerSize - offsesSize,
			ios::cur
	);

	StringInfoIndexs stringInfoIndexs;

	int offset = 0;
	ResTable_entry entry;
	Res_value value;
	for(int i = 0 ; i<header.entryCount ; i++) {
		if(pOffsets[i] == ResTable_type::NO_ENTRY) {
			continue;
		}
		resources.seekg(
				pOffsets[i] - offset,
						ios::cur
		);
		resources.read(
				(char*)&entry,
				sizeof(ResTable_entry)
		);
		resources.read(
				(char*)&value,
				sizeof(Res_value)
		);

		stringInfoIndexs.push_back(
				StringInfoIndex(entry.key.index, value.data)
		);

		offset = pOffsets[i] + sizeof(ResTable_entry) + sizeof(Res_value);
	}

	resources.seekg(
			header.header.size - header.entriesStart - offset,
			ios::cur
	);

	delete[] pOffsets;
	(*pStringInfoIndexInConfigs)[header.config.toString()] = stringInfoIndexs;
	return true;
}

bool ResourcesParser::parseStringFromPackage(
		ifstream& resources,
		const vector<string>& keys,
		int stringId,
		const string& packageName) {
	ResChunk_header chunkHeader;
	ResTable_type typeHeader;

	StringInfoIndexInConfigs stringInfoIndexInConfigs;
	while(resources.read((char*)&chunkHeader, sizeof(ResChunk_header))) {
		if(chunkHeader.type == RES_TABLE_PACKAGE_TYPE) {
			resources.seekg(
					-sizeof(ResChunk_header),
					ios::cur
			);
			return true;
		} else if(chunkHeader.type == RES_TABLE_TYPE_TYPE) {
			typeHeader.header = chunkHeader;
			resources.read(
					(char*)&typeHeader + sizeof(ResChunk_header),
					sizeof(ResTable_type) - sizeof(ResChunk_header)
			);

			if(typeHeader.id != stringId) {
				resources.seekg(
						chunkHeader.size - sizeof(ResTable_type),
						ios::cur
				);

				continue;
			}

			resources.seekg(
					chunkHeader.headerSize - sizeof(ResTable_type),
					ios::cur
			);

			if(!parseStringFromResTable(
						resources,
						keys,
					   	typeHeader,
						&stringInfoIndexInConfigs)) {
				return false;
			}
		} else {
			resources.seekg(
					chunkHeader.size - sizeof(ResChunk_header),
					ios::cur
			);
		}
	}
	mStringInfoIndexInPackage[packageName] = stringInfoIndexInConfigs;
	return true;
}
