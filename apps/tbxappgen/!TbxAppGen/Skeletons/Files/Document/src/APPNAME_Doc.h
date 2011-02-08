
#ifndef %APPNAME%DOC_H_
#define %APPNAME%DOC_H_

#include "%APPNAME%FileType.h"
#include "tbx/doc/document.h"
#include <string>

/**
 * Add to this class to create the data for the document
 */
class %APPNAME%Doc : public tbx::doc::Document
{
public:
	%APPNAME%Doc();
	virtual ~%APPNAME%Doc();

	/**
	 * Return the file type for this document
	 */
	virtual int file_type() const {return %APPNAME%_FILE_TYPE;}

	/**
	 * Return the size of the document.
	 *
	 * If the size cannot be easily calculated return an
	 * estimate
	 */
	virtual int document_size() const
	{
	   //TODO: Change 1024 to the actual size of the documnet
	   return 1024;
	}


	virtual bool save(std::ostream &os);
	virtual bool load(std::istream &is, int estimated_size);
};

#endif /* %APPNAME%DOC_H_ */
