/*
 * logging.cc
 *
 *  Created on: 12 Nov 2010
 *      Author: alanb
 */

#include "logging.h"
#include "tbx/path.h"

#include <iostream>

bool logging = false;

std::ofstream log;

void start_logging()
{
	if (!logging)
	{
		int log_id = 0;
		char name[32];
		while (!logging)
		{
			log_id++;
			sprintf(name, "<SaverTest$Dir>.log%d", log_id);
			tbx::Path p(name);
			if (!p.exists())
			{
				log.open(name);
				logging = log.is_open();
			}
		}
	}
}
