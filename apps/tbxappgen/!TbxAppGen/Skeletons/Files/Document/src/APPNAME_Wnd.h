
#ifndef %APPNAME%WND_H_
#define %APPNAME%WND_H_

#include "%APPNAME%Doc.h"
#include "tbx/doc/docwindow.h"
#include "tbx/doc/docpositionwindow.h"


/**
 * Class to display and interact with the document through
 * a window.
 */
class %APPNAME%Wnd :
	public tbx::doc::DocWindow
{
private:
    // This object is used to move each successive window
    // down. Remove it if you want the windows to start
    // in the same place
	static tbx::doc::DocPositionWindow _start_pos;

public:
	%APPNAME%Wnd(%APPNAME%Doc *doc);
	virtual ~%APPNAME%Wnd();

    // Typed access to the document this window is showing/editing
	%APPNAME%Doc *document() {return static_cast<%APPNAME%Doc *>(_document);}
	%APPNAME%Doc *document() const {return static_cast<%APPNAME%Doc *>(_document);}
};

#endif /* %APPNAME%WND_H_ */
