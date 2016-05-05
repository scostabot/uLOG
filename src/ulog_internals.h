/** uLOG - micro logging library
 ** A very light customizable logging library for program tracing/debugging
 **
 ** Doubly licensed BSD 3-clause/LGPLv2.1 see LICENSE.TXT
 ** (c) 2015 mestesso.it
 **/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#ifndef ULOG_REMOVE_TIMEDENTRY
#include <time.h>
#endif

typedef struct {
	unsigned int timedentry:1;
} ulog_flags_t;

typedef unsigned int ulog_mask_t;

#define ULOG_TRACE_ALL   0xFFFFFFFF
#define ULOG_TRACE_MAIN  1
#define ULOG_TRACE_NONE  0

typedef struct {
	char *name;                 /* The name of trace module */
	ulog_mask_t mask;           /* Bitmask associated to given module */
} ulog_module_t;

typedef struct ulog_module_list_t {
	struct ulog_module_list_t *next; /* Point next module, otherwise NULL */
	ulog_module_t *mod;
} ulog_module_list_t;

typedef enum {
	ulog_no_method,
	ulog_output_stdout,
	ulog_output_stderr,
	ulog_output_file,
	ulog_output_previous
} ulog_method_t;

typedef struct {
	char *starting;
	char *ending;
	char *r_separator;
	char *value;
	char *l_separator;
} ulog_array_t;

typedef struct {
	ulog_method_t method;          /* Current trace method */
	ulog_method_t previous_method; /* Previously defined  method */

	char *userfile;                /* If trace goes to a file, path */
	FILE *stream;                  /* Stream associated to above */

	char *ulog_format;             /* Log output format string */
	ulog_array_t array_format;     /* String array format string */

	ulog_flags_t flags;            /* Library flags */

	ulog_mask_t mask;              /* Trace module visibility mask */
	ulog_module_list_t *module_el;
} ulog_decl_t;

#define ULOG(t...)       {\
	extern ulog_decl_t _ulog_decl;\
	extern ulog_module_t  _ulog_main_module;\
	extern ulog_module_t* _ulog_current_module;\
	_ulog((_ulog_current_module == NULL) ? &_ulog_main_module : _ulog_current_module, __FILE__, __PRETTY_FUNCTION__, __LINE__, t);\
}

#define ULOGA(t,type,n)      {\
	extern ulog_decl_t _ulog_decl;\
	extern ulog_module_t  _ulog_main_module;\
	extern ulog_module_t* _ulog_current_module;\
        _uloga((_ulog_current_module == NULL) ? &_ulog_main_module : _ulog_current_module, __FILE__, __PRETTY_FUNCTION__, __LINE__, t, type, n);\
}

#define ULOG_AFORMAT(st, lse, v, rse, end)  _ulog_aformat(st, lse, v, rse, end)
#define ULOG_DISABLE()        _ulog_disable()
#define ULOG_ENABLE(a)        _ulog_enable(a)
#ifndef ULOG_REMOVE_FILEOUTPUT
#define ULOG_FILE(f)          _ulog_file(f)
#else
#warning "You are using ULOG_FILE but file I/O is disabled"
#endif
#define ULOG_FLAGS(f)         _ulog_flags(f)
#define ULOG_FORMAT(f)        _ulog_format(f)
#define ULOG_LEVEL_ADD(l)     _ulog_level_add(l)
#define ULOG_LEVEL_GET(l)     _ulog_level_get(l)
#define ULOG_LEVEL_SET(l)     _ulog_level_set(l)
#define ULOG_MODULE_ADD(m)    _ulog_module_add(m)

#ifdef ULOG_CALLS_INLINE
	#define ULOG_AFORMAT_INLINE
	#define ULOG_DISABLE_INLINE
	#define ULOG_ENABLE_INLINE
	#define ULOG_FILE_INLINE
	#define ULOG_FLAGS_INLINE
	#define ULOG_FORMAT_INLINE
	#define ULOG_LEVEL_ADD_INLINE
	#define ULOG_LEVEL_GET_INLINE
	#define ULOG_LEVEL_SET_INLINE
	#define ULOG_MODULE_ADD_INLINE
	#define ULOG_INLINE
	#define ULOGA_INLINE
#endif

#define ULOG_DECL ulog_module_t _ulog_main_module = { "MAIN", 1 };\
                  ulog_module_t* _ulog_current_module = &_ulog_main_module;\
                  ulog_module_list_t _ulog_module_list = { NULL, &_ulog_main_module };\
                  ulog_decl_t _ulog_decl = { ulog_no_method, ulog_no_method, NULL, NULL,\
                  ULOG_FORMAT_DEFAULT, { "[", "]", ",", "%s", "" }, { 0 }, 1, &_ulog_module_list };

#define ULOG_MODULE_DECL(m)  static ulog_module_t* _ulog_current_module = m;

#ifndef ULOG_REMOVE_TIMEDENTRY
static inline void _ulog_timed_entry(const char *msg) {
	extern ulog_decl_t _ulog_decl;
	time_t t;
	struct tm tm;

	t = time(NULL);
	tm = *localtime(&t);
	fprintf(_ulog_decl.stream, "%s%d-%d-%d %d:%d:%d", msg, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}
#endif

/**
 ** Internal functions
 ** Functions defined in this section cannot be called outside
 ** this file. They are internally used functions and there is
 ** no guarantee that exported interfaces remain stable between
 ** releses.
 **
 **/

/** _ulog_printf
 ** Implement the way a uLOG trace is formatted
 **
 **/
static inline char * _ulog_printf(ulog_module_t *m, const char *file,
                                  const char *func, unsigned int l) 
{
	extern ulog_decl_t _ulog_decl;
	char *p = _ulog_decl.ulog_format; 
	int msg = 0;

	while(((*p != '\0') && (!msg)))
	{ 
		if(*p != '%')
			fputc(*p, _ulog_decl.stream);
		else {
			switch(*++p)
			{
			case 'b': // b function block
				fprintf(_ulog_decl.stream, "%s", func);
				break;
			case 'f': // f file
				fprintf(_ulog_decl.stream, "%s", file);
				break;
			case 'l': // l line number
				fprintf(_ulog_decl.stream, "%d", l);
				break;
			case 'm': // m module name
				fprintf(_ulog_decl.stream, "%s", m->name);
				break;
#ifndef ULOG_REMOVE_TIMEDENTRY
			case 't': // t time
				_ulog_timed_entry("");
				break;
#endif
			case '\0':
			case 'v': // v application message
				msg = 1;
				break;
			}
		}

		p++;
	}

	return p;
}
 
/**
 ** End of internal functions section.
 **
 **/

/**
 ** uLOG public API
 **
 **/

/** ULOG_AFORMAT(const char *start, const char *lsep,
 **              const char *rsep, const char *end)
 **
 ** Define the uLOG output format string for ULOGA().
 ** - start: the pointer to a string that will be printed
 **   before outputting the ULOGA() argument(s).
 ** - lsep: the left separator string. This string will
 **         be printed at the left of each non-NULL
 **         element of the given string array.
 ** - rsep: the right separator string. This string will
 **         be printed at the right of each non-NULL
 **         element of the given string array.
 ** - end:  this string will be printed after the last
 **         element of the given string array. 
 ** 
 **                                                     **/

#ifdef ULOG_AFORMAT_INLINE
static inline
#endif
void _ulog_aformat(const char *start, const char *lsep, const char *value,
                   const char *rsep, const char *end)
#ifdef ULOG_LIBH
;
#else
{
	extern ulog_decl_t _ulog_decl;

	_ulog_decl.array_format.starting =    (char *) start;
	_ulog_decl.array_format.l_separator = (char *) lsep;
	_ulog_decl.array_format.value = (char *) value;
	_ulog_decl.array_format.r_separator = (char *) rsep;
	_ulog_decl.array_format.ending =      (char *) end;
}
#endif

/** ULOG_DISABLE()
 ** Deactivate uLOG library regardless of current
 ** output method.
 **                                                     **/

#ifdef ULOG_DISABLE_INLINE
static inline
#endif
void _ulog_disable(void)
#ifdef ULOG_LIBH
;
#else
{
	extern ulog_decl_t _ulog_decl;

#ifndef ULOG_REMOVE_TIMEDENTRY
	if (_ulog_decl.method && _ulog_decl.flags.timedentry) {
		_ulog_timed_entry("Log ended at: ");
		fprintf(_ulog_decl.stream, "\n");
	}
#endif

	switch(_ulog_decl.method) {
#ifndef ULOG_REMOVE_FILEOUTPUT
	case ulog_output_file:
		fclose(_ulog_decl.stream);
		_ulog_decl.stream = NULL;
		break;
#endif
	default:
		break;
	}

	_ulog_decl.previous_method = _ulog_decl.method;
	_ulog_decl.method = ulog_no_method;
}
#endif

/** ULOG_ENABLE(ulog_method_t output method)
 **
 ** Activate uLOG library using the given output method.
 ** <output method> is one of the following:
 **
 **   ulog_output_file
 **      uLOG trace output goes to a file. The pathname
 **      is defined via ULOG_FILE() call
 **
 **   ulog_output_previous
 **      uLOG trace output goes to the last configured channel
 **
 **   ulog_output_stderr
 **      uLOG trace output goes to standard error
 **
 **   ulog_output_stdout
 **      uLOG trace output goes to standard output
 **
 **   if the output method is unrecognized, standard
 **   error will be used as default output method.
 **                                                     **/

#ifdef ULOG_ENABLE_INLINE
static inline
#endif
void _ulog_enable(ulog_method_t method)
#ifdef ULOG_LIBH
;
#else
{
	extern ulog_decl_t _ulog_decl;

	if (method == ulog_no_method)
		return;

	if (method == ulog_output_previous)
		method = _ulog_decl.previous_method;

	switch(method) {
#ifndef ULOG_REMOVE_FILEOUTPUT
	case ulog_output_file:
		if (_ulog_decl.userfile == NULL)
			_ulog_decl.stream = stderr;
		else if ((_ulog_decl.stream = fopen(_ulog_decl.userfile, "a+")) == NULL) {
			perror("ULOG");
			return;
		}
		break;
#endif
	case ulog_output_stdout:
		_ulog_decl.stream = stdout;
		break;
	case ulog_output_stderr:
	default:
		_ulog_decl.stream = stderr;
	}

	_ulog_decl.method = method;

#ifndef ULOG_REMOVE_TIMEDENTRY
	if (_ulog_decl.flags.timedentry) {
		_ulog_timed_entry("Log started at: ");
		fprintf(_ulog_decl.stream, "\n");
	}
#endif
}
#endif

/** ULOG_FILE(const char *pathname)
 ** Set the pathname used to store logging if uLOG is
 ** instructed to output to a file.
 **                                                     **/

#ifndef ULOG_REMOVE_FILEOUTPUT
#ifdef ULOG_FILE_INLINE
static inline
#endif
void _ulog_file(const char *path)
#ifdef ULOG_LIBH
;
#else
{
	extern ulog_decl_t _ulog_decl;
	_ulog_decl.userfile = (char *) path;
}
#endif
#endif

/** ULOG_FLAGS(_ulog_flags_t flags)
 **
 ** Set uLOG lib option flags. Caller can set/reset
 ** this set of boolean values in order to change 
 ** default library behaviour.
 ** Available flags:
 **
 **   - timedentry: if true a timestamp is outputted
 **     each time ULOG_DISABLE and ULOG_ENABLE are
 **     called, so you can know when trace logging
 **     is started or ended (regardless if timestamp
 **     is enabled or not in ULOG_FORMAT()).
 ** 
 **                                                     **/

#ifdef ULOG_FLAGS_INLINE
static inline
#endif
void _ulog_flags(ulog_flags_t flags)
#ifdef ULOG_LIBH
;
#else
{
	extern ulog_decl_t _ulog_decl;

	_ulog_decl.flags = flags;
}
#endif

/** ULOG_FORMAT(const char *fmt)
 **
 ** Define the uLOG output format string.
 ** Default value is defined in ULOG_FORMAT_DEFAULT.
 ** (normally "%f:%l <%v>\n").
 ** the format string behaves as xprintf() standard
 ** library call which wraps to, excluding this
 ** proprietary flags:
 **
 **  - %f: print the filename of ULOGx() call
 **  - %l: print the line number of ULOGx() call
 **  - %t: print the timestamp of ULOGx() call
 **  - %v: print the message inside ULOGx() call
 ** 
 **                                                     **/

#ifdef ULOG_FORMAT_INLINE
static inline
#endif
void _ulog_format(const char *fmt)
#ifdef ULOG_LIBH
;
#else
{
	extern ulog_decl_t _ulog_decl;

	_ulog_decl.ulog_format = (char *) fmt;
}
#endif

/** _ulog_level **/

#ifdef ULOG_LEVEL_ADD_INLINE
static inline
#endif
void _ulog_level_add(const unsigned int m)
#ifdef ULOG_LIBH
;
#else
{
	extern ulog_decl_t _ulog_decl;

	if (m) /* We forbid to disable implicitely logs */
		_ulog_decl.mask |= m;
}
#endif

#ifdef ULOG_LEVEL_GET_INLINE
static inline
#endif
unsigned int _ulog_level_get(const unsigned int m)
#ifdef ULOG_LIBH
;
#else
{
	extern ulog_decl_t _ulog_decl;

	return _ulog_decl.mask;
}
#endif

#ifdef ULOG_LEVEL_SET_INLINE
static inline
#endif
void _ulog_level_set(const unsigned int m)
#ifdef ULOG_LIBH
;
#else
{
	extern ulog_decl_t _ulog_decl;

	_ulog_decl.mask = m;
}
#endif

#ifdef ULOG_MODULE_ADD_INLINE
static inline
#endif
void _ulog_module_add(ulog_module_t *m)
#ifdef ULOG_LIBH
;
#else
{
	extern ulog_decl_t _ulog_decl;
	ulog_module_list_t *mod = _ulog_decl.module_el->next;

	while(mod != NULL) {
		mod = mod->next;
	}

	mod->mod = m;
	mod->next = NULL;
}
#endif

/** ULOG(ulog_module_t *m, const char *sourcefile, const char *function,
 **      unsigned int line, ...)
 **
 ** Emit a trace for module m, at given sourcefile name, within
 ** given function positioned at given line. A variable list
 ** of argument(s) follows, if any.
 ** 
 **                                                              **/

#ifdef ULOG_INLINE
static inline
#endif
void _ulog(ulog_module_t *m, const char *sourcefile, const char *function,
           unsigned int line, ...)
#ifdef ULOG_LIBH
;
#else
{
	extern ulog_decl_t _ulog_decl;
	char *p;
	char *pfmt;
	va_list ap;

	if (_ulog_decl.method && (_ulog_decl.mask & m->mask)) {
		va_start(ap, line);

		switch(_ulog_decl.method) {
		case ulog_output_file:
		case ulog_output_stderr:
		case ulog_output_stdout:
			pfmt = _ulog_printf(m, sourcefile, function, line);
			p = va_arg(ap, char *);
			vfprintf(_ulog_decl.stream, p, ap);
			fprintf(_ulog_decl.stream, "%s", pfmt);
			break;
		default:
			break;
		}

		va_end(ap);
	}
}
#endif

/** ULOGA(ulog_module_t *m, const char *sourcefile, const char *function,
 **       unsigned int line, const char *array, int t, unsigned int n)
 **
 ** Emit a trace for module m, at given sourcefile name, within
 ** given function positioned at given line, then output the
 ** given array, each element of type t for a number of bytes n.
 **
 ** Type t can have the follwong values:
 **   - 0: an array of strings is given in input
 **   - 1: an array of bytes is given in input
 **   - 2: an array of short words is given in input
 **   - 4: an array of integers is given in input
 **
 ** If n is zero means that the ULOGA() call output array 
 ** elements until finds one which is zero-valued.
 ** 
 **                                                             **/

#ifdef ULOGA_INLINE
static inline
#endif
void _uloga(ulog_module_t *m, const char *sourcefile, const char *function,
            unsigned int line, const void *array, int t, unsigned int n)
#ifdef ULOG_LIBH
;
#else
{
	extern ulog_decl_t _ulog_decl;
	static char elem_str[16] = { '%', 's' };
	char *pfmt;
	char **as;
	unsigned char *pb;
	unsigned short *ps;
	unsigned int *pi;
	unsigned int *value;
	unsigned int condition;
	unsigned int i;

	if (_ulog_decl.method && (_ulog_decl.mask & m->mask)) {
		switch(t) {
		case 0:  as = (char **) array; break;
		case sizeof(char):  pb = (unsigned char *) array; break;
		case sizeof(short):  ps = (unsigned short *) array; break;
		case sizeof(int):  pi = (unsigned int *) array; break;
		default: pb = (unsigned char *) array; break;
		}

		if (n)
			i = n;

		pfmt = _ulog_printf(m, sourcefile, function, line);
		fprintf(_ulog_decl.stream, "%s",
		        _ulog_decl.array_format.starting);
		strcpy(&elem_str[2], _ulog_decl.array_format.value);

		while (1) {
			switch(t) {
			case 0: value = (unsigned int *) *as++; break;
			case sizeof(char): value = (unsigned int*) *pb++; break;
			case sizeof(short): value = (unsigned int*) *ps++; break;
			case sizeof(int):  value = (unsigned int*) *pi++; break;
			default: value = (unsigned int*) *pb++; break;
			}

			condition = n ? i-- : (unsigned int) value;
			if (condition) {
				fprintf(_ulog_decl.stream, elem_str,
				        _ulog_decl.array_format.l_separator,
				        value);
				if (condition > 1)
					fprintf(_ulog_decl.stream, "%s",
				        	_ulog_decl.array_format.r_separator);
			} else
				break;
		}

		fprintf(_ulog_decl.stream, "%s%s",
		        _ulog_decl.array_format.ending, pfmt);
	}
}
#endif
