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

#include "log.h"

#include <iostream>
#include <log4cplus/configurator.h>

namespace nutpp {
namespace util {
// Create the static instance of Log and return a reference to it.
Log &Log::getInstance()
{
    // Guaranteed to be destroyed.
    static Log instance;
    // Instantiated on first use.
    return instance;
}

// Initialize Log with the provided configuration file
bool Log::initialize(const std::string &app_dir,
                     const std::string &log_cfg, std::string &log_file)
{
    try {
        log4cplus::initialize();

        // Load static configuration.
        log4cplus::helpers::Properties properties(app_dir + "/" + log_cfg);

        // Apply dynamic configuration.
        // ${AppDir} placeholder can be used in the logger configuration to
        // to set the log file location relative to the installation directory.
        properties.setProperty(LOG4CPLUS_TEXT("AppDir"), app_dir);

        // Configure logger.
        log4cplus::PropertyConfigurator prop_conf(
            properties, log4cplus::Logger::getDefaultHierarchy(),
            log4cplus::PropertyConfigurator::fShadowEnvironment);
        prop_conf.configure();

        if (log4cplus::Logger::exists(LOG4CPLUS_TEXT("CoreLogger"))) {
            core_logger_
                = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("CoreLogger"));
            log_file = prop_conf.getProperties()
                       .getProperty(LOG4CPLUS_TEXT("appender.CORE.File"));

            return true;
        }

        core_logger_ = log4cplus::Logger::getRoot();
        //TODO: create default appender to stderr
    } catch (const std::exception &e) {
        std::cerr << "Logging initialization failed :"
                  << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Logging initialization failed with unknown exception."
                  << std::endl;
    }

    return false;
}

// Manual cleanup of internal logger resources.
void Log::shutdown()
{
    core_logger_.closeNestedAppenders();
}
} // namespace util
} // namespace nutpp
