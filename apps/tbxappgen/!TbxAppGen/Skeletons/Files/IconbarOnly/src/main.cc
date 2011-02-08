
#include "tbx/application.h"
#include "tbx/reporterror.h"
#include <kernel.h>
#include <iostream>

int main()
{
	tbx::Application my_app("<%APPNAME%$Dir>");

	my_app.run();

	return 0;
}
