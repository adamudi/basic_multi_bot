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
