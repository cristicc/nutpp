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

#include "nutpp/log.h"

#include <log4cplus/configurator.h>
#include <log4cplus/fileappender.h>

#include <iostream>

namespace nutpp {
namespace log {
// Create the static instance of Log and return a reference to it.
Log &Log::GetInstance()
{
    static Log instance;
    return instance;
}

// Initialize Log with the provided configuration file
bool Log::Initialize(const char *application_path,
                     const char *log_cfg)
{
    // Make sure we have an instance of Log created before we initialize it.
    GetInstance();

    try {
        log4cplus::initialize();

        // Load static configuration.
        log4cplus::helpers::Properties properties(log_cfg);

        // Apply dynamic configuration.
        // ${AppDir} placeholder can be used in the logger configuration to
        // to set the log file location relative to the installation directory.
        properties.setProperty(LOG4CPLUS_TEXT("AppDir"), application_path);

        // Configure logger.
        log4cplus::PropertyConfigurator property_configurator(
            properties,
            log4cplus::Logger::getDefaultHierarchy(),
            log4cplus::PropertyConfigurator::fShadowEnvironment);
        property_configurator.configure();
    } catch (const std::exception &e) {
        std::cerr << "Logging initialization failed :" << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Logging initialization failed with unknown exception."
                  << std::endl;
        return false;
    }

    return true;
}

// Initialize internal loggers.
Log::Log()
{
    core_logger_ = log4cplus::Logger::getInstance("Logger");
}

// Manual cleanup of internal logger resources.
void Log::Shutdown()
{
    core_logger_.closeNestedAppenders();
}
} // namespace log
} // namespace nutpp
