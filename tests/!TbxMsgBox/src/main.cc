
#include "tbx/application.h"
#include "tbx/reporterror.h"
#include "tbx/command.h"
#include <kernel.h>
#include <iostream>
#include "tbx/messagewindow.h"

/**
 * Class to make it easy to add a message to a command
 */
class ShowMessage : public tbx::Command
{
    std::string _msg;
    std::string _title;
    std::string _sprite;

    public:
      ShowMessage(const std::string &msg, const std::string &title ="", const std::string sprite="") : _msg(msg), _title(title), _sprite(sprite) {}

      virtual void execute()
      {
           tbx::show_message(_msg,_title, _sprite);
      }
};

/**
 * Class to use report_error from a command
 */
class ReportError : public tbx::Command
{
    std::string _msg;

    public:
      ReportError(const std::string &msg) : _msg(msg) {}

      virtual void execute()
      {
           tbx::report_error(_msg);
      }
};

/**
 * Main entry point to the application
 */
int main()
{
	tbx::Application my_app("<TbxMsgBox$Dir>");

    my_app.add_command(1, new ShowMessage("Short test"));
    my_app.add_command(2, new ShowMessage("This is a much, much longer test to see if the word wrapping stuff works ok"));
    my_app.add_command(3, new ShowMessage(
        "This is a test of using linefeeds to break\n"
        "a message into multiple lines.\n\n"
        "Above was two linefeeds together to test adding a blank line.\n"
        "I've also added enough lines so that\n"
        "The message window has to be increased in height."
      ));

    my_app.add_command(4, new ShowMessage("This tests changing the title and sprite\nIn this case for an error",
    "Error - somethings wrong",
    "error"));
    my_app.add_command(5, new ShowMessage("This tests changing the title and sprite\nIn this case for a warning",
    "Warning something may be wrong",
    "warning"));

    my_app.add_command(11, new ReportError("Short Error test"));
    my_app.add_command(12, new ReportError("This is a much, much longer test of report error to see if it does any word wrapping"));

	my_app.run();

	return 0;
}
