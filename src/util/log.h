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

#ifndef NUTPP_UTIL_LOG_H_
#define NUTPP_UTIL_LOG_H_

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

/**
 * @brief Holds a reference to the core logger object.
 *
 * @def LOGNUTPP_CORE_LOGGER
 */
#define LOGNUTPP_CORE_LOGGER    nutpp::util::Log::getInstance().coreLogger()

/**
 * @brief Inserts a TRACE entry in the log file. Use this for the most detailed
 * logging possible.
 *
 * @def LOGNUTPP_TRACE
 */
#define LOGNUTPP_TRACE(logEvent) LOG4CPLUS_TRACE(LOGNUTPP_CORE_LOGGER, logEvent)

/**
 * @brief Inserts a DEBUG entry in the log file. Use this for detailed, but not
 * essential logging.
 *
 * @def LOGNUTPP_DEBUG
 */
#define LOGNUTPP_DEBUG(logEvent) LOG4CPLUS_DEBUG(LOGNUTPP_CORE_LOGGER, logEvent)

/**
 * @brief Inserts an INFO entry in the log file. Use this for important
 * information output, flow steps and other relevant data. Normally this is the
 * logging level used in production environments.
 *
 * @def LOGNUTPP_INFO
 */
#define LOGNUTPP_INFO(logEvent)  LOG4CPLUS_INFO(LOGNUTPP_CORE_LOGGER, logEvent)

/**
 * @brief Inserts an WARN entry in the log file. Use this for error cases with
 * low severity and that do not impact the normal application logic flow.
 *
 * @def LOGNUTPP_WARN
 */
#define LOGNUTPP_WARN(logEvent)  LOG4CPLUS_WARN(LOGNUTPP_CORE_LOGGER, logEvent)

/**
 * @brief Inserts an ERROR entry in the log file. Use this for error cases with
 * high severity that impact the normal application logic flow, but are absorbed
 * by the application in a stable way.
 *
 * @def LOGNUTPP_ERROR
 */
#define LOGNUTPP_ERROR(logEvent) LOG4CPLUS_ERROR(LOGNUTPP_CORE_LOGGER, logEvent)

/**
 * @brief Inserts an FATAL entry in the log file. Use this for error cases with
 * terminating severity that make the normal application logic flow impossible;
 * usually the application should stop right after logging on this level.
 *
 * @def LOGNUTPP_FATAL
 */
#define LOGNUTPP_FATAL(logEvent) LOG4CPLUS_FATAL(LOGNUTPP_CORE_LOGGER, logEvent)

/**
 * @namespace nutpp
 * @brief Project root namespace.
 */
namespace nutpp {
/**
 * @namespace nutpp::util
 * @brief Namespace containing utilities.
 */
namespace util {
/**
 * @class Log
 * @brief General purpose logging class.
 *
 * This class implements a singleton wrapper over log4cplus functionality. It
 * exposes two types of loggers, a core logger for general purpose application
 * logging (https://sourceforge.net/p/log4cplus/wiki/Home/).
 *
 * @warning Do not use this class directly in your code. Access the logging
 * functionality through the @c LOGNUTPP_* macros. Every source file containing
 * logging code must contain one of the @c LOGNUTPP_LOGGER_* macros before any
 * @c LOGNUTPP_* macro is called. They introduce the logger object, to be used
 * for all the logging code in that source file.
 *
 * @see LOGNUTPP_LOGGER_CORE LOGNUTPP_TRACE LOGNUTPP_DEBUG
 * LOGNUTPP_INFO LOGNUTPP_WARN LOGNUTPP_ERROR LOGNUTPP_FATAL
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
     * @see LOGNUTPP_LOGGER_CORE
     *
     * @warning Use this function in your code only once before initializing
     * logging and once before cleanup. Besides these it is only called by the
     * @c LOGNUTPP_LOGGER_* macros.
     */
    static Log &getInstance();

    /**
     * @brief Initializes this instance of @c Log.
     *
     * @param[in] app_dir Path to the application root folder.
     * @param[in] log_cfg Path of the log4cplus configuration file,
     * relative to @c app_dir.
     * @param[out] log_file Path to the application log file.
     *
     * @return @c true if the initialization succeeded. The path of
     * the log file will be returned in @c log_file.
     *
     * @see log4cplus.properties
     *
     * @warning This function should be called only once at the very beginning
     * of the main application logic.
     */
    bool initialize(const std::string &app_dir,
                    const std::string &log_cfg,
                    std::string &log_file);

    /**
     * @brief Accessor to the core logger.
     *
     * @return Reference to a @c log4cplus::Logger object that should be used
     * for core operations logging.
     *
     * @see LOGNUTPP_LOGGER_CORE
     *
     * @warning Do not use this function in your code, it is called by the
     * @c LOGNUTPP_LOGGER_CORE macro.
     */
    log4cplus::Logger &coreLogger() { return core_logger_; }

    /**
     * @brief Manual cleanup of internal logger resources.
     *
     * @warning Call this method if you need to perform additional operation on
     * resources that are affected by the logger component (E.g. remove folder
     * containing log files).
     */
    void shutdown();

private:
    /**
     * @brief Constructor is kept private to prevent direct instantiation.
     *
     * @see getInstance()
     */
    Log() = default;

    /**
     * @brief Destructor is kept private to prevent direct instantiation.
     *
     * @see getInstance()
     */
    ~Log() = default;

    /// log4cplus::Logger object used for logging core related messages.
    log4cplus::Logger core_logger_;
};
} // namespace util
} // namespace nutpp
#endif /* NUTPP_UTIL_LOG_H_ */
