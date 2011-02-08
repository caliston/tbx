
#include "%APPNAME%Doc.h"

%APPNAME%Doc::%APPNAME%Doc()
{
}

%APPNAME%Doc::~%APPNAME%Doc()
{
}

/**
 * Save the document
 */
bool %APPNAME%Doc::save(std::ostream &os)
{
	//TODO: Write code to save the document here
	// and return true if the save was OK.
	return true;
}

/**
 * Load the document
 */
bool %APPNAME%Doc::load(std::istream &is, int estimated_size)
{
	//TODO: Write code to load the document here
	//Note: estimate_size could be -1 if it is unknown
	return !is.bad();
}
