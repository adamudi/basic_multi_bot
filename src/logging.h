/*
Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#pragma once

#include <string>
#include <iostream>
#include <cstring>

#define STANDARD (1 << 0)
#define SOCKET (1 << 1)
#define OTHER (1 << 2)

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

