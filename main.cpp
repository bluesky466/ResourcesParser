#include "ResourcesParser.h"

#include <iostream>

using namespace std;

#define ID(x) (x + 1)

int main(int argc, char *argv[]) {
	ResourcesParser parser(argv[1]);
	for(auto it : parser.getResourceForPackageName()) {
		cout<<it.first<<endl;
		ResourcesParser::ResStringPoolPtr types = it.second->pTypes;
		ResourcesParser::ResStringPoolPtr keys = it.second->pKeys;
		for(uint32_t i = 0 ; i < types->header.stringCount ; i++) {
			string type = ResourcesParser::getStringFromResStringPool(types, i);
			cout<<"\t"<<type<<endl;
			for(ResourcesParser::ResTableTypePtr pResTableType : it.second->resTablePtrs[ID(i)]) {
				for(int j = 0 ; j < pResTableType->entries.size() ; j++) {
					if(pResTableType->entries[j] == nullptr){
						continue;
					}
					if(j==0) {
						string config = pResTableType->header.config.toString();
						cout<<"\t\t"<<type<<(config.length()>0?"-":"")<<config<<endl;
					}
					cout<<"\t\t\t"<<(int)pResTableType->values[j]->dataType
						<<"   "
						<<ResourcesParser::getStringFromResStringPool(keys, pResTableType->entries[j]->key.index)
						<<" = "
						<<parser.stringOfValue(pResTableType->values[j])
						<<endl;
				}
			}
		}
	}

	return 0;
}
