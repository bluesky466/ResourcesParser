rp : \
	main.cpp \
	ResourcesParserInterpreter.h \
	ResourcesParserInterpreter.cpp \
	ResourcesParser.h \
	ResourcesParser.cpp \
	ResourceTypes.h \
	ResourceTypes.cpp \
	configuration.h \
	ByteOrder.h
	g++ main.cpp ResourcesParserInterpreter.cpp ResourcesParser.cpp ResourceTypes.cpp -std=c++11 -o rp

.PHONY : clean
clean :
	rm rp

