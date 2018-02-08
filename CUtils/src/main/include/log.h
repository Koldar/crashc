/**
 * \file log.h
 *
 * Tiny logging utility
 *
 * This header provides you a simple, but effective, debugging logging utility.
 *
 * The module provides several levels of logging:
 * \li debug;
 * \li finest;
 * \li finer;
 * \li fine;
 * \li info;
 * \li warning;
 * \li error;
 * \li critical;
 *
 * Each of them has 3 functions you can use to logging (X stands for one of the logging levels)
 *
 * \li <tt>X(format, ...)</tt>: use to log something with a particular level;
 * \li <tt>gX(cmds, format, ...)</tt> (*g* stands for "general"): use if you want to execute something before actuallly logging. If the log is disabled, cmds are not executed at all;
 * \li <tt>bX(format, ...)</tt>(*b* stands for "buffer"): provides a buffer of char you can use only in your logging code. The buffer is named \c log_buffer.
 *
 *  Created on: Dec 23, 2016
 *      Author: koldar
 */

#ifndef LOG_H_
#define LOG_H_

/**
 * Enable this to improve log performance
 *
 * Define this macro with the level you want to use.
 * This will make the logger slightly quicker, but will disable the runtime ::setLevel call
 * Use only if you don't care about the runtime level at all.
 *
 * \note
 * You should set this flag \b before including this header, otherwise nothing will happen
 *
 * \example
 * Use this to trash away logs like ::LogLevel::LOG_DEBUG or ::LogLevel::LOG_FINE
 */
#ifdef QUICK_LOG
#endif

#define __LOG_ALL		0
#define __LOG_DEBUG		1
#define __LOG_FINEST	2
#define __LOG_FINER		3
#define __LOG_FINE		4
#define __LOG_INFO		5
#define __LOG_WARNING	6
#define __LOG_ERROR		7
#define __LOG_CRITICAL	8
#define __LOG_OFF 		9

/**
 * All the available log levels
 *
 * \attention
 * Do not assign numbers to the levels, otheriwse ::LOG_LEVEL_SIZE won't work anymore
 */
typedef enum LogLevel {
	LOG_ALL = __LOG_ALL,
	LOG_DEBUG = __LOG_DEBUG,
	LOG_FINEST = __LOG_FINEST,
	LOG_FINER = __LOG_FINER,
	LOG_FINE = __LOG_FINE,
	LOG_INFO = __LOG_INFO,
	LOG_WARNING = __LOG_WARNING,
	LOG_ERROR = __LOG_ERROR,
	LOG_CRITICAL = __LOG_CRITICAL,
	LOG_OFF = __LOG_OFF
} LogLevel;
///the number of log levels in ::LogLevel
#define  LOG_LEVEL_SIZE 10

/**
 * Represents the type of the function we need to feed to ::addAppenders
 *
 * The typedef defines what exactly is an appender.
 *
 * \def An appender is whatever function that, given:
 * 	\li the file where the log has been called;
 * 	\li the function where the log has been called;
 * 	\li the line involved;
 * 	\li the ::LogLevel the log has been called on;
 * 	\li the message string the developer wants to store
 *
 * actually adds to the physical log (i.e. a database, a file, the stdout) the log entry.
 */
typedef void (*appenderFunction)(const char*, const char*, const int, const LogLevel, const char*);

/**
 * @param[in] level the log leve of the log to print
 * @param[in] absoluteFilePath the absoliute path of the file *.c contanining this log call
 * @param[in] functionName the name of the function containing this log call
 * @param[in] lineNo the line number inside \c absoluteFilePath where this log call is positioned
 * @param[in] logBufferSize size of the buffer c logBuffer
 * @param[inout] logBuffer a buffer that we will use to generate the log string
 * @param[in] format the \c printf string template of the log message (and only message)
 * @param[in] ... variadic argument building the log message it self
 */
void __generic_log_function(const LogLevel level, const char* absoluteFilePath, const char* functionName, const int lineNo, int logBufferSize, char* logBuffer, const char* format, ...);
void setLevel(LogLevel level);
void excludeLogger(const char* logger);
void includeLogger(const char* logger);
void clearExcludedLoggers();

/**
 * Clear all the appenders added up until now
 */
void clearAppenders();
/**
 * Add a new appender to the appenders list
 *
 * \todo at the moment, only an appender per time is available
 */
void addAppender(appenderFunction);

#ifndef LOG_BUFFER
#	define LOG_BUFFER __cutils_log_buffer
#endif
#define log_buffer LOG_BUFFER

#ifndef LOG_BUFFER_SIZE
#	define LOG_BUFFER_SIZE 300
#endif

extern char __default_log_buffer[LOG_BUFFER_SIZE];


/**
 * Before doing the actual log, we need to execute some commands
 *
 * @param[in] function the log function to use
 * @param[in] cmd a list of ";" separated C statements to execute before calling the log
 * @param[in] format the \c printf string template of the arguments of the log
 * @param[in] ... variadic argument of \c printf
 */
#define CUTILS_LOG_GLOG(cmds, logLevel, bufferSize, buffer, format, ...) \
		{ \
			cmds; \
			__generic_log_function(logLevel, __FILE__, __func__, __LINE__, bufferSize, buffer, format, ## __VA_ARGS__); \
		}

/**
 * Like a normal log, but within this log you can exploit a char buffer variable called \c log_buffer inside the message
 *
 * Typical use is the following one:
 *
 * ```
 * binfo("The list is %s", (getBufferStringOfList(l, log_buffer, default_bufferstring), log_buffer));
 * ```
 *
 * @param[in] logLevel the level of the log to print out
 * @param[in] format the \c printf string template of the arguments of the log
 * @param[in] ... variadic argument of \c printf
 */
#define CUTILS_LOG_BLOG(logLevel, format, ...) CUTILS_LOG_SBLOG(logLevel, LOG_BUFFER_SIZE, format, ## __VA_ARGS__)

/**
 * Like ::CUTILS_LOG_BLOG, but you can specify the size of the temporary buffer \c log_buffer as well
 *
 * Typical use is when you need to print out a very long message but the default buffer size is too small.
 * Instead of tweaking ::LOG_BUFFER_SIZE macro at compilation time (making **all** the log buffer bigger),
 * you expliclty set a biffer buffer **only for your** particular buffer.
 *
 * @param[in] logLevel the level of the log to print out
 * @param[in] logSize the size of the buffer \c log_buffer
 * @param[in] format the \c printf string template of the arguments of the log
 * @param[in] ... variadic argument of \c printf
 */
#define CUTILS_LOG_SBLOG(logLevel, logSize, format, ...) \
	CUTILS_LOG_GLOG( \
		char log_buffer[logSize]; char output_buffer[logSize], \
		logLevel, \
		logSize, \
		output_buffer, \
		format, ## __VA_ARGS__ \
	)


#if QUICK_LOG > __LOG_DEBUG
#	pragma message "DEBUG LOG TURNED OFF"
#	define gdebug(cmds, format, ...) 0
#	define bdebug(format, ...) 0
#	define sbdebug(size, format, ...) 0
#	define debug(format,...) 0
#else
#	define gdebug(cmds, format, ...) CUTILS_LOG_GLOG(cmds, LOG_DEBUG, LOG_BUFFER_SIZE, __default_log_buffer, format, ## __VA_ARGS__)
#	define bdebug(format, ...) CUTILS_LOG_BLOG(LOG_DEBUG, format, ## __VA_ARGS__)
#	define sbdebug(size, format, ...) CUTILS_LOG_SBLOG(LOG_DEBUG, size, format, ## __VA_ARGS__)
#	define debug(format,...) __generic_log_function(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, LOG_BUFFER_SIZE, __default_log_buffer, format, ##__VA_ARGS__)
#endif

#if QUICK_LOG > __LOG_FINEST
#	pragma message "FINEST LOG TURNED OFF"
#	define gfinest(cmds, format, ...) 0
#	define bfinest(format, ...) 0
#	define sbfinest(size, format, ...) 0
#	define finest(format,...) 0
#else
#	define gfinest(cmds, format, ...) CUTILS_LOG_GLOG(cmds, LOG_FINEST, LOG_BUFFER_SIZE, __default_log_buffer, format, ## __VA_ARGS__)
#	define bfinest(format, ...) CUTILS_LOG_BLOG(LOG_FINEST, format, ## __VA_ARGS__)
#	define sbfinest(size, format, ...) CUTILS_LOG_SBLOG(LOG_FINEST, size, format, ## __VA_ARGS__)
#	define finest(format,...) __generic_log_function(LOG_FINEST, __FILE__, __FUNCTION__, __LINE__, LOG_BUFFER_SIZE, __default_log_buffer, format, ##__VA_ARGS__)
#endif

#if QUICK_LOG > __LOG_FINER
#	pragma message "FINER LOG TURNED OFF"
#	define gfiner(cmds, format, ...) 0
#	define bfiner(format, ...) 0
#	define sbfiner(size, format, ...) 0
#	define finer(format,...) 0
#else
#	define gfiner(cmds, format, ...) CUTILS_LOG_GLOG(cmds, LOG_FINER, LOG_BUFFER_SIZE, __default_log_buffer, format, ## __VA_ARGS__)
#	define bfiner(format, ...) CUTILS_LOG_BLOG(LOG_FINER, format, ## __VA_ARGS__)
#	define sbfiner(size, format, ...) CUTILS_LOG_SBLOG(LOG_FINER, size, format, ## __VA_ARGS__)
#	define finer(format,...) __generic_log_function(LOG_FINER, __FILE__, __FUNCTION__, __LINE__, LOG_BUFFER_SIZE, __default_log_buffer, format, ##__VA_ARGS__)
#endif

#if QUICK_LOG > __LOG_FINE
#	pragma message "FINE LOG TURNED OFF"
#	define gfine(cmds, format, ...) 0
#	define bfine(format, ...) 0
#	define sbfine(size, format, ...) 0
#	define fine(format,...) 0
#else
#	define gfine(cmds, format, ...) CUTILS_LOG_GLOG(cmds, LOG_FINE, LOG_BUFFER_SIZE, __default_log_buffer, format, ## __VA_ARGS__)
#	define bfine(format, ...) CUTILS_LOG_BLOG(LOG_FINE, format, ## __VA_ARGS__)
#	define sbfine(size, format, ...) CUTILS_LOG_SBLOG(LOG_FINE, size, format, ## __VA_ARGS__)
#	define fine(format,...) __generic_log_function(LOG_FINE, __FILE__, __FUNCTION__, __LINE__, LOG_BUFFER_SIZE, __default_log_buffer, format, ##__VA_ARGS__)
#endif

#if QUICK_LOG > __LOG_INFO
#	pragma message "INFO LOG TURNED OFF"
#	define ginfo(cmds, format, ...) 0
#	define binfo(format, ...) 0
#	define sbinfo(size, format, ...) 0
#	define info(format,...) 0
#else
#	define ginfo(cmds, format, ...) CUTILS_LOG_GLOG(cmds, LOG_INFO, LOG_BUFFER_SIZE, __default_log_buffer, format, ## __VA_ARGS__)
#	define binfo(format, ...) CUTILS_LOG_BLOG(LOG_INFO, format, ## __VA_ARGS__)
#	define sbinfo(size, format, ...) CUTILS_LOG_SBLOG(LOG_INFO, size, format, ## __VA_ARGS__)
#	define info(format,...) __generic_log_function(LOG_INFO, __FILE__, __FUNCTION__, __LINE__, LOG_BUFFER_SIZE, __default_log_buffer, format, ##__VA_ARGS__)
#endif

#if QUICK_LOG > __LOG_WARNING
#	pragma message "WARNING LOG TURNED OFF"
#	define gwarning(cmds, format, ...) 0
#	define bwarning(format, ...) 0
#	define sbwarning(size, format, ...) 0
#	define warning(format,...) 0
#else
#	define gwarning(cmds, format, ...) CUTILS_LOG_GLOG(cmds, LOG_WARNING, LOG_BUFFER_SIZE, __default_log_buffer, format, ## __VA_ARGS__)
#	define bwarning(format, ...) CUTILS_LOG_BLOG(LOG_WARNING, format, ## __VA_ARGS__)
#	define sbwarning(size, format, ...) CUTILS_LOG_SBLOG(LOG_WARNING, size, format, ## __VA_ARGS__)
#	define warning(format,...) __generic_log_function(LOG_WARNING, __FILE__, __FUNCTION__, __LINE__, LOG_BUFFER_SIZE, __default_log_buffer, format, ##__VA_ARGS__)
#endif

#if QUICK_LOG > __LOG_ERROR
#	pragma message "ERROR LOG TURNED OFF"
#	define gerror(cmds, format, ...) 0
#	define berror(format, ...) 0
#	define sberror(size, format, ...) 0
#	define error(format,...) 0
#else
#	define gerror(cmds, format, ...) CUTILS_LOG_GLOG(cmds, LOG_ERROR, LOG_BUFFER_SIZE, __default_log_buffer, format, ## __VA_ARGS__)
#	define berror(format, ...) CUTILS_LOG_BLOG(LOG_ERROR, format, ## __VA_ARGS__)
#	define sberror(size, format, ...) CUTILS_LOG_SBLOG(LOG_ERROR, size, format, ## __VA_ARGS__)
#	define error(format,...) __generic_log_function(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__, LOG_BUFFER_SIZE, __default_log_buffer, format, ##__VA_ARGS__)
#endif

#if QUICK_LOG > __LOG_CRITICAL
#	pragma message "CRTICAL LOG TURNED OFF"
#	define gcritical(cmds, format, ...) 0
#	define bcritical(format, ...) 0
#	define sbcritical(size, format, ...) 0
#	define critical(format,...) 0
#else
#	define gcritical(cmds, format, ...) CUTILS_LOG_GLOG(cmds, LOG_CRITICAL, LOG_BUFFER_SIZE, __default_log_buffer, format, ## __VA_ARGS__)
#	define bcritical(format, ...) CUTILS_LOG_BLOG(LOG_CRITICAL, format, ## __VA_ARGS__)
#	define sbcritical(size, format, ...) CUTILS_LOG_SBLOG(LOG_CRITICAL, size, format, ## __VA_ARGS__)
#	define critical(format,...) __generic_log_function(LOG_CRITICAL, __FILE__, __FUNCTION__, __LINE__, LOG_BUFFER_SIZE, __default_log_buffer, format, ##__VA_ARGS__)
#endif

#endif /* LOG_H_ */


