/*
 * logging.h
 *
 *  Created on: 12 Nov 2010
 *      Author: alanb
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#include <fstream>

extern bool logging;
extern std::ofstream log;

void start_logging();

#endif /* LOGGING_H_ */
