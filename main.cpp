#include "ResourcesParser.h"

#include <sstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	ResourcesParser parser(argv[1]);
	ResourcesParser::StringInfoIndexInPackage::const_iterator packageIt
		 = parser.getStringInfoIndexInPackage().begin();

	const vector<string>& strings = parser.getGlobalStringPool();
	ResourcesParser::StringKeyInPackage keysInPackage
		= parser.getStringKeyInPackage();

	while(packageIt != parser.getStringInfoIndexInPackage().end()) {
		const vector<string>& keys = keysInPackage[packageIt->first];
		ResourcesParser::StringInfoIndexInConfigs::const_iterator configIt
			= packageIt->second.begin();
		while(configIt != packageIt->second.end()) {
			cout<<"value-"<<configIt->first.c_str()<<endl;
			ResourcesParser::StringInfoIndexs::const_iterator indexIt
				= configIt->second.begin();
			while(indexIt != configIt->second.end()) {
				cout
					<< "\t"
					<< keys[indexIt->keyIndex]
					<< " -> "
					<<	strings[indexIt->valueIndex]
					<< endl;
				indexIt++;
			}
			configIt++;
		}
		packageIt++;
	}

	return 0;
}
