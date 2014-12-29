/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */

#pragma once

#include <string>
#include <iostream>
#include <cstring>

#define MASK_SHIFT(offset) (1 << offset)

#define STANDARD MASK_SHIFT(0)
#define SOCKET MASK_SHIFT(1)
#define OTHER MASK_SHIFT(2)
#define IRC MASK_SHIFT(3)
#define XMPP MASK_SHIFT(4)

#ifndef LOG_MASK // If LOG_MASK isn't defined, dont log anything but standard
#define LOG_MASK (STANDARD)
#endif

/**
 * Print a log message prefixed with "LOG <file>:<lineno>: " to
 * stdout. It will only print if any of the log types defined by the
 * first parameter are set to true in LOG_MASK. Example usage:
 * 
 * log(OTHER | SOCKET, "log if either OTHER or SOCKET is true");
 */
#define log(filter, cformat, ...) if (((filter) & (LOG_MASK)) != 0) printf("LOG %s:%u: " cformat "\n", __FILE__, __LINE__, ##__VA_ARGS__)

/**
 * Print a log message prefixed with "LOG <file>:<lineno>: " to
 * stdout. It will only print if all of the log types defined by the
 * first parameter are set to true in LOG_MASK. Example usage:
 * 
 * log_all(OTHER | SOCKET, "log if both OTHER or SOCKET are true");
 */
#define log_all(filter, cformat, ...) if (((filter) & (LOG_MASK)) == (filter)) printf("LOG %s:%u: " cformat "\n", __FILE__, __LINE__, ##__VA_ARGS__)

/**
 * Print a log message prefixed with "LOG <file>:<lineno>: " to
 * stderr. It will only print if any of the log types defined by the
 * first parameter are set to true in LOG_MASK. Example usage:
 * 
 * log(OTHER | SOCKET, "log if either OTHER or SOCKET is true");
 */
#define elog(filter, cformat, ...) if (((filter) & (LOG_MASK)) != 0) fprintf(stderr, "LOG %s:%u: " cformat "\n", __FILE__, __LINE__, ##__VA_ARGS__)

/**
 * Print a log message prefixed with "LOG <file>:<lineno>: " to
 * stderr. It will only print if all of the log types defined by the
 * first parameter are set to true in LOG_MASK. Example usage:
 * 
 * log_all(OTHER | SOCKET, "log if both OTHER or SOCKET are true");
 */
#define elog_all(filter, cformat, ...) if (((filter) & (LOG_MASK)) == (filter)) fprintf(stderr, "LOG %s:%u: " cformat "\n", __FILE__, __LINE__, ##__VA_ARGS__)

/**
 * Print a log message prefixed with "LOG <file>:<lineno>: " to
 * stdout. It will only print if any of the log types defined by the
 * first parameter are set to true in LOG_MASK. Example usage:
 */
#define slog(filter, ...) if (((filter) & (LOG_MASK)) != 0) std::cout << "LOG " << __FILE__ << ":" << __LINE__ << ": " << __VA_ARGS__ << "\n"

/**
 * Print a log message prefixed with "LOG <file>:<lineno>: " to
 * stdout. It will only print if all of the log types defined by the
 * first parameter are set to true in LOG_MASK. Example usage:
 */
#define slog_all(filter, ...) if (((filter) & (LOG_MASK)) == (filter)) std::cout << "LOG " << __FILE__ << ":" << __LINE__ << ": " << __VA_ARGS__ << "\n"

/**
 * Print a log message prefixed with "LOG <file>:<lineno>: " to
 * stderr. It will only print if any of the log types defined by the
 * first parameter are set to true in LOG_MASK. Example usage:
 */
#define selog(filter, ...) if (((filter) & (LOG_MASK)) != 0) std::cerr << "LOG " << __FILE__ << ":" << __LINE__ << ": " << __VA_ARGS__ << "\n"

/**
 * Print a log message prefixed with "LOG <file>:<lineno>: " to
 * stderr. It will only print if all of the log types defined by the
 * first parameter are set to true in LOG_MASK. Example usage:
 */
#define selog_all(filter, ...) if (((filter) & (LOG_MASK)) == (filter)) std::cerr << "LOG " << __FILE__ << ":" << __LINE__ << ": " << __VA_ARGS__ << "\n"

