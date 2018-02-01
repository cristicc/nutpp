/**
 * nutpp - a nutrition planning utility
 *
 * Copyright (C) 2018  Cristian Ciocaltea <cristian.ciocaltea@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef CA_INCLUDE_LOG_H_
#define CA_INCLUDE_LOG_H_

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

/**
 * @namespace nutpp
 * @brief Project root namespace.
 */
namespace nutpp {
/**
 * @namespace nutpp::log
 * @brief Namespace containing logging utilities.
 */
namespace log {
/**
 * @class Log
 * @brief General purpose logging class.
 *
 * This class implements a singleton wrapper over log4cplus functionality. It
 * exposes two types of loggers, a core logger for general purpose application
 * logging (https://sourceforge.net/p/log4cplus/wiki/Home/).
 *
 * @warning Do not use this class directly in your code. Access the logging
 * functionality through the @c LOG4NUT_* macros. Every source file containing
 * logging code must contain one of the @c LOG4NUT_LOGGER_* macros before any
 * @c LOG4NUT_* macro is called. They introduce the logger object, to be used
 * for all the logging code in that source file.
 *
 * @see LOG4NUT_LOGGER_CORE LOG4NUT_TRACE LOG4NUT_DEBUG
 * LOG4NUT_INFO LOG4NUT_WARN LOG4NUT_ERROR LOG4NUT_FATAL
 */
class Log {
public:
    /// Copy now allowed.
    Log(const Log &) = delete;
    /// Assignment not allowed.
    Log &operator=(const Log &) = delete;

    /**
     * @brief Accessor to the single instance of this class.
     *
     * @return Reference to the single instance of @c Log.
     *
     * @see LOG4NUT_LOGGER_CORE
     *
     * @warning Use this function in your code only once before initializing
     * logging and once before cleanup. Besides these it is only called by the
     * @c LOG4NUT_LOGGER_* macros.
     */
    static Log &GetInstance();

    /**
     * @brief Initializes this instance of @c Log.
     *
     * @param[in] application_path Folder path where the application using
     * logging is installed in. Usually it is the folder containing the common
     * sub-folders bin, lib and conf.
     * @param[in] log_cfg File path of the log4cplus configuration file.
     *
     * @return @c true if the initialization succeeded.
     *
     * @see log4cplus.properties
     *
     * @warning This function should be called only once at the very beginning
     * of main application logic.
     */
    static bool Initialize(const char *application_path, const char *log_cfg);

    /**
     * @brief Accessor to the core logger.
     *
     * @return Reference to a @c log4cplus::Logger object that should be used
     * for core operations logging.
     *
     * @see LOG4NUT_LOGGER_CORE
     *
     * @warning Do not use this function in your code, it is called by the
     * @c LOG4NUT_LOGGER_CORE macro.
     */
    inline log4cplus::Logger &core_logger() { return core_logger_; }

    /**
     * @brief Manual cleanup of internal logger resources.
     *
     * @warning Call this method if you need to perform additional operation on
     * resources that are affected by the logger component (E.g. remove folder
     * containing log files).
     */
    void Shutdown();

private:
    /**
     * @brief Constructor is kept private to prevent direct instantiation.
     *
     * @see GetInstance()
     */
    Log();

    /**
     * @brief Destructor is kept private to prevent direct instantiation.
     *
     * @see GetInstance()
     */
    ~Log() = default;

    /// log4cplus::Logger object used for general purpose logging.
    log4cplus::Logger core_logger_;
};

/**
 * Enables the usage of the core logger in the source file where it is present.
 *
 * @def LOG4NUT_LOGGER_CORE
 */
#define LOG4NUT_LOGGER_CORE() \
    static log4cplus::Logger & logger \
        = nutpp::log::Log::GetInstance().core_logger()

/**
 * @brief Inserts a TRACE entry in the log file. Use this for the most detailed
 * logging possible.
 *
 * @def LOG4NUT_TRACE
 */
#define LOG4NUT_TRACE(logEvent) LOG4CPLUS_TRACE(logger, logEvent)

/**
 * @brief Inserts a DEBUG entry in the log file. Use this for detailed, but not
 * essential logging.
 *
 * @def LOG4NUT_DEBUG
 */
#define LOG4NUT_DEBUG(logEvent) LOG4CPLUS_DEBUG(logger, logEvent)

/**
 * @brief Inserts an INFO entry in the log file. Use this for important
 * information output, flow steps and other relevant data. Normally this is the
 * logging level used in production environments.
 *
 * @def LOG4NUT_INFO
 */
#define LOG4NUT_INFO(logEvent)  LOG4CPLUS_INFO(logger, logEvent)

/**
 * @brief Inserts an WARN entry in the log file. Use this for error cases with
 * low severity and that do not impact the normal application logic flow.
 *
 * @def LOG4NUT_WARN
 */
#define LOG4NUT_WARN(logEvent)  LOG4CPLUS_WARN(logger, logEvent)

/**
 * @brief Inserts an ERROR entry in the log file. Use this for error cases with
 * high severity that impact the normal application logic flow, but are absorbed
 * by the application in a stable way.
 *
 * @def LOG4NUT_ERROR
 */
#define LOG4NUT_ERROR(logEvent) LOG4CPLUS_ERROR(logger, logEvent)

/**
 * @brief Inserts an FATAL entry in the log file. Use this for error cases with
 * terminating severity that make the normal application logic flow impossible;
 * usually the application should stop right after logging on this level.
 *
 * @def LOG4NUT_FATAL
 */
#define LOG4NUT_FATAL(logEvent) LOG4CPLUS_FATAL(logger, logEvent)
} // namespace log
} // namespace nutpp
#endif /* CA_INCLUDE_LOG_H_ */
