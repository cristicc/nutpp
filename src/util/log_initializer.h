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

#ifndef NUTPP_UTIL_LOG_INITIALIZER_H_
#define NUTPP_UTIL_LOG_INITIALIZER_H_

#include <memory>
#include <string>
#include <log4cplus/loglevel.h>

namespace nutpp {
/**
 * @brief Namespace containing general purpose utilities.
 */
namespace util {
/**
 * @brief Helper class to configure the logging subsystem.
 *
 * This class implements a wrapper over standard @e log4cplus functionality.
 * It handles the logger initialization, configuration and automatic shutdown
 * when the instance gets out of scope, which is normally the end of @c main().
 *
 * @warning Make sure the class instance is preserved during the entire
 * execution time of the application.
 *
 * @see log4cplus::Initialize for more details about the logger init process.
 */
class LogInitializer {
public:
    /// Constructor.
    LogInitializer();

    /// Copy constructor not allowed.
    LogInitializer(LogInitializer const &) = delete;
    /// Move constructor not allowed.
    LogInitializer(LogInitializer &&) = delete;
    /// Assignment not allowed.
    LogInitializer &operator=(LogInitializer const &) = delete;

    /// Move assignment allowed.
    LogInitializer &operator=(LogInitializer &&) = delete;

    /// Destructor.
    ~LogInitializer();

    /**
     * @brief Initializes this instance of @c Log.
     *
     * @param[in] app_dir Path to the application root folder.
     * @param[in] log_cfg Path of the log4cplus configuration file,
     * relative to @c app_dir.
     * @param[out] log_file Path to the application log file.
     *
     * @return @c true if the initialization succeeded and the path of
     * the log file stored in @c log_file.
     *
     * @warning This function should be called only once at the very beginning
     * of the main application logic, before using any of the logging macros.
     */
    bool configure(const std::string &app_dir,
                   const std::string &log_cfg,
                   std::string &log_file);

    /// Default logging pattern for the console appender.
    static constexpr const char *kDefaultConsoleLogPattern
        = "%d{%Y-%m-%d %H:%M:%S.%q} %-5p %m%n";

    /// Default log level for the console logger.
    static const log4cplus::LogLevel kDefaultConsoleLogLevel
        = log4cplus::INFO_LOG_LEVEL;

private:
    // Hide implementation details.
    struct LogInitializerImpl;
    std::unique_ptr<LogInitializerImpl> impl_;
};
} // namespace util
} // namespace nutpp
#endif /* NUTPP_UTIL_LOG_INITIALIZER_H_ */
