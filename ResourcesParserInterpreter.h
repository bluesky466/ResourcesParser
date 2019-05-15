#ifndef RESOURCES_PARSER_INTERPRETER_H
#define RESOURCES_PARSER_INTERPRETER_H

#include "ResourceTypes.h"
#include "ResourcesParser.h"

#include <string>

class ResourcesParserInterpreter {
public:
	static const std::string ALL_TYPE;
	static const std::string ATTR_TYPE;
	static const std::string DRAWABLE_TYPE;
	static const std::string MIPMAP_TYPE;
	static const std::string LAYOUT_TYPE;
	static const std::string ANIM_TYPE;
	static const std::string ANIMATOR_TYPE;
	static const std::string XML_TYPE;
	static const std::string DIMEN_TYPE;
	static const std::string STRING_TYPE;
	static const std::string STYLE_TYPE;
	static const std::string COLOR_TYPE;
	static const std::string ID_TYPE;
	static const std::string INTEGER_TYPE;

	ResourcesParserInterpreter(ResourcesParser* parser) : mParser(parser) {  }

	static std::string getConfigDirectory(const ResTable_config& config, const std::string& type) {
		std::string str = config.toString();
		return type + (str.length()>0  ? "-" : "") + str;
	}

	void parserResource(const std::string& type);

	void parserId(const std::string& id);

private:
	ResourcesParser* mParser;

	void parserEntry(
		uint32_t resId,
		ResourcesParser::ResStringPoolPtr pKeys,
		ResTable_entry* pEntry,
		Res_value* pValue,
		const std::string& type,
		const std::string& tab);

	void parserResource(
		ResourcesParser::PackageResourcePtr packageRes,
		uint32_t typeId,
		const std::string& type,
		const std::string& tab);
};

#endif  /*RESOURCES_PARSER_INTERPRETER_H*/
