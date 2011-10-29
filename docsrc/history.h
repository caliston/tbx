/**
 * @page history History/Changelog
 *
 * <B>0.1 Alpha June 2010</B>
 * - Alpha release so library used with !PackMan is available
 *
 * <B>0.2 Alpha July 2010</B>
 * - Release so version used with PackIt is available.
 * - Removed uncaught exception handling so program stops with an error.
 * - Automatically remove file loaders from an object when it is destroyed.
 * - ReportView didn't refresh enough lines when items were inserted.
 * - All toolbox and Window WIMP events information now derives from EventInfo class
 * - Added missing toolbox objects ColourMenu, FileInfo, FontDbox, FontMenu,
 * PrintDbox, ProgInfo and Scale.
 * - Partial implementation of ColourDbox
 * - Added missing gadgets Adjuster, PopUp, RadioButton, Button, Draggable and ToolAction.
 * - WimpFont::paint - changed to paint font along base line provided
 * - Desktop font item rendering and textview rendering modifed to use 40 os units for the height.
 * - Added key pressed event processing to Window object.
 * - Fixed bug where command added to application could be executed multiple times
 *
 * <B>0.3 Alpha February 2011</B>
 * - Release so version used with PackIt 0.2 is available
 * - Added res namespace containing Toolbox Resource file/object/gadget handling
 * - WriteableField renamed to WritableField.
 * - Method/parameter names for file names modified to have consistent naming throughout the library
 * - Added functions to support message files
 * - Added doc namespace containing classes to help in writing document based applications
 * - Added static methods for accessing/setting a files file type.
 * - Added raw_file_type Path/PathInfo method to get file type for ImageFS files
 * - Added Opener handlers to application to be used for opening a file double clicked in the filer.
 * - Added Window scroll request listener
 * - Split out Wimp Message Listener to use listeners explicit for the three message types.
 * - Added drag methods to Window
 * - Updated ItemView to improve paging.
 * - Added mode changed and palette changed listeners to application.
 * - Various minor API and documentation fixes/improvements.
 * - fixed equals_ignore_case function as it failed when both parameters were strings.
 * - Added defer_delete template function to delay deletion of an object until the next null poll.
 * - Added UserSprite methods to set and get pixels.
 *
 * <B>0.4 Alpha September 2011</B>
 * - New userguide
 * - Added missing reference documentation on classes/methods etc.
 * - Minor class tidying
 *
 * <B>0.5 Alpha ???? </B>
 * - const char * constructor for Path class
 * - pointer operator for Path and PathInfo iterators
 * - Added foreground and background methods for setting the font colours in the TextView
 * - Removed space at beginning of a wrapped line in the TextView
 * - Added Point/Size constructor the BBox
 * - Missing ActionButton, Button, NumberRange, Slider and StringSet methods added
 * - JPEG added draw transform plot and file checking methods
 */
