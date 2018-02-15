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

#include "log_initializer.h"

#include <iostream>
#include <log4cplus/configurator.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/initializer.h>

namespace nutpp {
namespace util {
/**
 * @brief Hides the implementation details from the logger API.
 */
struct LogInitializer::LogInitializerImpl {
    /// The @e log4cplus specific initializer helper.
    log4cplus::Initializer initializer;
};

// C-tor.
LogInitializer::LogInitializer(log4cplus::LogLevel level)
    : consoleLogLevel_(level),
    impl_(std::make_unique<LogInitializer::LogInitializerImpl>())
{}

// Destructor in .cpp required by unique_ptr to avoid incomplete type errors.
LogInitializer::~LogInitializer() = default;

// Initialize Log with the provided configuration file
bool LogInitializer::configure(
    const std::string &log_cfg,
    const std::string &app_dir)
{
    try {
        // Load static configuration.
        log4cplus::helpers::Properties properties(log_cfg);

        // Apply dynamic configuration - replace ${AppDir}.
        properties.setProperty(LOG4CPLUS_TEXT("AppDir"), app_dir);

        // Configure logger.
        log4cplus::PropertyConfigurator prop_conf(
            properties, log4cplus::Logger::getDefaultHierarchy(),
            log4cplus::PropertyConfigurator::fShadowEnvironment);
        prop_conf.configure();

        if (!log4cplus::Logger::exists(LOG4CPLUS_TEXT("CORE"))) {
            // Enable logging to stderr.
            log4cplus::SharedAppenderPtr console(
                new log4cplus::ConsoleAppender(true));
            console->setLayout(
                std::make_unique<log4cplus::PatternLayout>(
                    kDefaultConsoleLogPattern));
            log4cplus::Logger::getRoot().addAppender(console);
            log4cplus::Logger::getRoot().setLogLevel(consoleLogLevel_);
            std::cerr << "Core logger not found, using default"
                      << " console appender to stderr" << std::endl;
        }

        return true;
    } catch (const std::exception &e) {
        std::cerr << "Logging initialization failed :"
                  << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Logging initialization failed with unknown exception."
                  << std::endl;
    }

    return false;
}
} // namespace util
} // namespace nutpp
