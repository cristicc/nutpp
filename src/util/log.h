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
 *
 * @file
 * @brief General purpose logger.
 */

#ifndef NUTPP_UTIL_LOG_H_
#define NUTPP_UTIL_LOG_H_

#include <log4cplus/loggingmacros.h>

/**
 * @brief Enables the usage of the logger for the given module @a m
 * in the source file where it is present.
 *
 * The module name should be registered in the app logger configuration,
 * typically @e log4cplus.properties.
 * Logger configuration sample for a fake module named @e MyModule:
 *
 * @code
 * log4cplus.logger.MyModule=INFO,MyAppender
 * @endcode
 */
#define LOGNUTPP_LOGGER(m) \
    namespace nutpp { \
    static log4cplus::Logger logger = log4cplus::Logger::getInstance(m); \
    }

/**
 * @brief Enables the usage of the @e Webserver module logger
 * in the source file where it is present.
 */
#define LOGNUTPP_LOGGER_WS          LOGNUTPP_LOGGER("CORE.WS")

/**
 * @brief Enables the usage of the @e Storage module logger
 * in the source file where it is present.
 */
#define LOGNUTPP_LOGGER_STORAGE     LOGNUTPP_LOGGER("CORE.STORAGE")

/**
 * @brief Inserts a new entry in the log file, on @e TRACE level.
 * Use this for the most detailed information possible.
 */
#define LOGNUTPP_TRACE(logEvent)    LOG4CPLUS_TRACE(nutpp::logger, logEvent)

/**
 * @brief Inserts a new entry in the log file, on @e DEBUG level.
 * Use this for detailed, but not very important information.
 */
#define LOGNUTPP_DEBUG(logEvent)    LOG4CPLUS_DEBUG(nutpp::logger, logEvent)

/**
 * @brief Inserts a new entry in the log file, on @e INFO level.
 * Use this for important information output.
 */
#define LOGNUTPP_INFO(logEvent)     LOG4CPLUS_INFO(nutpp::logger, logEvent)

/**
 * @brief Inserts a new entry in the log file, on @e WARN level.
 * Use this for error messages with low severity which do not impact the
 * normal application logic flow.
 */
#define LOGNUTPP_WARN(logEvent)     LOG4CPLUS_WARN(nutpp::logger, logEvent)

/**
 * @brief Inserts a new entry in the log file, on @e ERROR level.
 * Use this for error messages with high severity that impact the normal
 * application logic flow, but are handled by the application in a stable way.
 */
#define LOGNUTPP_ERROR(logEvent)    LOG4CPLUS_ERROR(nutpp::logger, logEvent)

/**
 * @brief Inserts a new entry in the log file, on @e FATAL level.
 * Use this for error messages with critical severity that cannot be
 * handled automatically by the application.
 */
#define LOGNUTPP_FATAL(logEvent) LOG4CPLUS_FATAL(nutpp::logger, logEvent)

/**
 * @brief Evaluates to @c true if DEBUG level is enabled in the
 * active logger.
 */
#define LOGNUTPP_DEBUG_ENABLED \
    nutpp::logger.isEnabledFor(log4cplus::DEBUG_LOG_LEVEL)
#endif /* NUTPP_UTIL_LOG_H_ */
