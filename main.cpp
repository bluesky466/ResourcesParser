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
					auto pEntry = pResTableType->entries[j];
					if(pEntry->flags & ResTable_entry::FLAG_COMPLEX) {
						const ResTable_map_entry* pMapEntry = (const ResTable_map_entry*)pEntry;
						const ResTable_map* pMap = (ResTable_map*)pResTableType->values[j];
						cout<<"\t\t\t"
							<<ResourcesParser::getStringFromResStringPool(it.second->pKeys, pEntry->key.index)
							<<endl;
						for(int i = 0 ; i < pMapEntry->count ; i++) {
							if(!ResourcesParser::isTableMapForAttrDesc((pMap+i)->name)){
								cout<<"\t";
							}
							cout<<"\t\t\t\t"
								<<parser.getNameForResTableMap((pMap+i)->name)
								<<"("
								<<hex
								<<(pMap+i)->name.ident
								<<") "
								<<parser.getValueForResTableMap((pMap+i)->value)<<endl;
						}
					}else{
						cout<<"\t\t\t"
							<<ResourcesParser::getStringFromResStringPool(keys, pResTableType->entries[j]->key.index)
							<<" = "
							<<parser.stringOfValue(pResTableType->values[j])
							<<endl;
					}
				}
			}
		}
	}

	return 0;
}
