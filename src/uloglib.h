/** uLOG - micro logging library
 ** A very light customizable logging library for program tracing/debugging
 **
 ** Doubly licensed BSD 3-clause/LGPLv2.1 see LICENSE.TXT
 ** (c) 2015 mestesso.it
 **/

#ifndef __ULOGH__
#define __ULOGH__

#define ULOG_MAJOR_VERSION  0
#define ULOG_MINOR_VERSION  8
#define ULOG_BUGFIX_VERSION 0
#define ULOG_VERSION_STR    "0.8.0"

#define ULOG_FORMAT_DEFAULT     "%f:%l <%v>\n"
#define ULOG_FORMAT_DEFAULT_PRT "%f:%l <%v>\\n"

#ifdef ULOG_REMOVE

#define ULOG(t...)
#define ULOGA(t...)
#define ULOG_AFORMAT(f)
#define ULOG_DECL
#define ULOG_DISABLE()
#define ULOG_ENABLE(a)
#define ULOG_FILE(f)
#define ULOG_FLAGS(f)
#define ULOG_FORMAT(f)
#define ULOG_LEVEL(l)
#define ULOG_MODULE(m,n)
#define ULOG_REGISTER(b) 0

#else

#define ULOG_LIBH

#include <ulog_internals.h>

#endif   // ULOG_REMOVE
#endif   // __ULOGH__
