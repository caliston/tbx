
#include "tbx/application.h"
#include "tbx/doc/docmanager.h"
#include "tbx/doc/dociconbar.h"
#include "tbx/doc/docsaveas.h"
#include "tbx/doc/docfileinfo.h"

#include "%APPNAME%Doc.h"
#include "%APPNAME%Wnd.h"

int main(int argc, char *argv[])
{
	tbx::Application docex_app("<%APPNAME%$Dir>");
	// Class to create documents and windows for the given file type
	tbx::doc::DocCreator<%APPNAME%Doc, %APPNAME%Wnd>
	    doc_creator(%APPNAME%_FILE_TYPE);
	// Document manager to help with creation of documents and Quit processing
	tbx::doc::DocManager doc_manager(&doc_creator);
	// Standard processing for document application dialogues
	tbx::doc::DocIconbar doc_iconbar;
	tbx::doc::DocSaveAs doc_saveas;
	tbx::doc::DocFileInfo doc_fileinfo;

    // Load any files from the command line
	if (argc > 1) doc_manager.load_files(argc-1, argv+1);

    // Run the application
	docex_app.run();

	return 0;
}
