#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "17";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2010";
	static const char UBUNTU_VERSION_STYLE[] = "10.11";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 0;
	static const long BUILD = 237;
	static const long REVISION = 1270;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 340;
	#define RC_FILEVERSION 0,0,237,1270
	#define RC_FILEVERSION_STRING "0, 0, 237, 1270\0"
	static const char FULLVERSION_STRING[] = "0.0.237.1270";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 237;
	

}
#endif //VERSION_H
