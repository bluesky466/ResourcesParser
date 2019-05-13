#include "ResourcesParser.h"

#include <iostream>
#include <sstream>

using namespace std;

#define ID(x) (x + 1)

const string ALL_TYPE = "";

int findArgvIndex(const char* argv, char *argvs[], int count);
const char* getArgv(const char* argv, char *argvs[], int count);
string getConfigDirectory(const ResTable_config& config, const string& type);
void parserType(const ResourcesParser& parser, const string& type);
void parserResourceByType(
		const ResourcesParser& parser,
		ResourcesParser::PackageResourcePtr packageRes,
		uint32_t id,
		const string& type,
		const string& tab);
void parserEntry(
		const ResourcesParser& parser,
		ResourcesParser::ResStringPoolPtr pKeys,
		ResTable_entry* pEntry,
		Res_value* pValue,
		const string& tab);

int main(int argc, char *argv[]) {
	const char* path = getArgv("-r", argv, argc);
	const char* type = getArgv("-t", argv, argc);
	const char* id = getArgv("-i", argv, argc);
	int all = findArgvIndex("-a", argv, argc);

	if(nullptr == path) {
		cout <<string(argv[0]) <<" -p path [-a] [-t type] [-i id]" <<endl;
		cout <<"-p : set path of resources.arsc" <<endl;
		cout <<"-a : show all of resources.arsc" <<endl;
		cout <<"-t : select the type in resources.arsc to show" <<endl;
		cout <<"-i : select the id of resource to show" <<endl;
		return -1;
	}

	ResourcesParser parser(path);

	if(all >= 0) {
		parserType(parser, ALL_TYPE);
	}

	if(type) {
		parserType(parser, type);
	}

	if(id) {
		uint32_t uid;
		if(0 == strncmp(id, "0x", 2)) {
			istringstream(id)>>hex>>uid;
		} else {
			istringstream(id)>>uid;
		}
		vector<ResourcesParser::ResTableTypePtr> resTableTypePtrs
			= parser.getResTableTypesForId(uid);
		if(resTableTypePtrs.empty()) {
			cout <<"can't find resource for " <<id <<endl;
		} else {
			ResourcesParser::PackageResourcePtr pPackage = parser.getPackageResouceForId(uid);
			uint32_t typeId = TYPE_ID(uid);
			uint32_t entryId = ENTRY_ID(uid);
			string type = ResourcesParser::getStringFromResStringPool(pPackage->pTypes, typeId-1);
			for(ResourcesParser::ResTableTypePtr pResTableType : resTableTypePtrs) {
				ResTable_entry* pEntry = pResTableType->entries[entryId];
				Res_value* pValue = pResTableType->values[entryId];
				if(nullptr != pEntry) {
					cout <<getConfigDirectory(pResTableType->header.config, type) << " : ";
					parserEntry(parser, pPackage->pKeys, pEntry, pValue, "");
					cout <<endl;
				}
			}
		}
	}
	return 0;
}

int findArgvIndex(const char* argv, char *argvs[], int count) {
	for(int i = 0 ; i<count ; i++) {
		if(strcmp(argv, argvs[i])==0) {
			return i;
		}
	}
	return -1;
}

const char* getArgv(const char* argv, char *argvs[], int count) {
	int index = findArgvIndex(argv, argvs, count);
	if(index>=0 && index+1 < count) {
		return argvs[index+1];
	}
	return nullptr;
}


void parserEntry(
		const ResourcesParser& parser,
		ResourcesParser::ResStringPoolPtr pKeys,
		ResTable_entry* pEntry,
		Res_value* pValue,
		const string& tab) {
	string key = ResourcesParser::getStringFromResStringPool(pKeys, pEntry->key.index);
	if(pEntry->flags & ResTable_entry::FLAG_COMPLEX) {
		cout <<tab<<key<<endl;
		ResTable_map_entry* pMapEntry = (ResTable_map_entry*)pEntry;
		ResTable_map* pMap = (ResTable_map*)pValue;

		for(int i = 0 ; i < pMapEntry->count ; i++) {
			string name = parser.getNameForResTableMap((pMap+i)->name);
			string value = parser.getValueForResTableMap((pMap+i)->value);

			if(!ResourcesParser::isTableMapForAttrDesc((pMap+i)->name)){
				cout
					<<tab+"\t\t"
					<<name
					<<hex
					<<"(0x" <<(pMap+i)->name.ident <<") = "
					<<value
					<<endl;
			} else {
				cout <<tab+"\t" <<name <<hex <<"(0x" <<(pMap+i)->name.ident <<") " <<value <<endl;
			}
		}
	}else{
		string value = parser.stringOfValue(pValue);
		cout <<tab+"\t" <<key <<" = " <<value <<endl;
	}
}

string getConfigDirectory(const ResTable_config& config, const string& type) {
	string str = config.toString();
	return type + (str.length()>0  ? "-" : "") + str;
}

void parserResourceByType(
		const ResourcesParser& parser,
		ResourcesParser::PackageResourcePtr packageRes,
		uint32_t id,
		const string& type,
		const string& tab) {
	for(ResourcesParser::ResTableTypePtr pResTableType : packageRes->resTablePtrs[id]) {
		for(int i = 0 ; i < pResTableType->entries.size() ; i++) {
			if(pResTableType->entries[i] == nullptr){
				continue;
			}
			if(i==0) {
				string config = getConfigDirectory(pResTableType->header.config, type);
				cout<<endl<<tab<<config<<endl;
			}
			auto pEntry = pResTableType->entries[i];
			auto keys = packageRes->pKeys;
			parserEntry(
					parser,
					keys,
					pEntry,
					pResTableType->values[i],
					tab + "\t");
		}
	}
}

void parserType(const ResourcesParser& parser, const string& type) {
	for(auto it : parser.getResourceForPackageName()) {
		cout<<it.first<<endl;
		ResourcesParser::ResStringPoolPtr types = it.second->pTypes;
		for(uint32_t i = 0 ; i < types->header.stringCount ; i++) {
			string resType = ResourcesParser::getStringFromResStringPool(types, i);
			if(type==ALL_TYPE || type == resType){
				parserResourceByType(parser, it.second, ID(i), resType, "\t");
			}
		}
	}
}

