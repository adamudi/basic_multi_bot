/**
 * ISC License  Copyright (c) 2014, Tom Alexander <tom@fizz.buzz>
 */

/**
 * Test program for logging.h. With the defines set up in premake4.lua
 * the result should be:
 * 
 * LOG src/logging.cpp:35: log socket
 * LOG src/logging.cpp:36: log other
 * LOG src/logging.cpp:37: log both
 * LOG src/logging.cpp:39: log_all socket
 * LOG src/logging.cpp:40: log_all other
 * LOG src/logging.cpp:41: log_all both
 */
#include "logging.h"

int main(int argc, char** argv)
{
    log(STANDARD, "log standard");
    log_all(STANDARD, "log_all standard");

    log(SOCKET, "log socket");
    log(OTHER, "log other");
    log(OTHER | SOCKET, "log both");

    log_all(SOCKET, "log_all socket");
    log_all(OTHER, "log_all other");
    log_all(OTHER | SOCKET, "log_all both");

    return 0;
}
