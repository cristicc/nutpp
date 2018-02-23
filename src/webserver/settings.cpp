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

#include "settings.h"
#include "util/log.h"

#include <Wt/WServer.h>

LOGNUTPP_LOGGER_WS;

namespace nutpp {
namespace webserver {
// Reads a string from app configuration.
std::string readAppStringSetting(const char *name, const char *default_value)
{
    std::string result;

    if (!Wt::WServer::instance()->readConfigurationProperty(name, result)) {
        LOGNUTPP_WARN("Missing app setting: " << name);

        if (default_value) {
            result = default_value;
        }
    }

    LOGNUTPP_DEBUG("Using app setting: " << name << "=" << result);
    return result;
}

// Simplified version of readAppStringSetting().
std::string silentReadAppStringSetting(const char *name)
{
    std::string result;
    Wt::WServer::instance()->readConfigurationProperty(name, result);
    return result;
}

// Reads an int from app configuration.
int readAppIntSetting(const char *name, int default_value)
{
    int result = default_value;
    std::string propval;

    if (Wt::WServer::instance()->readConfigurationProperty(name, propval)) {
        try {
            result = std::stoi(propval);
        } catch (const std::exception &) {
            LOGNUTPP_WARN("Invalid app setting: " << name << "=" << propval);
        }
    } else {
        LOGNUTPP_WARN("Missing app setting: " << name);
    }

    LOGNUTPP_DEBUG("Using app setting: " << name << "=" << result);
    return result;
}

// Reads a bool from app configuration.
bool readAppBoolSetting(const char *name, bool default_value)
{
    bool result = default_value;
    std::string propval;

    if (Wt::WServer::instance()->readConfigurationProperty(name, propval)) {
        if (propval == "true") {
            result = true;
        } else if (propval == "false") {
            result = false;
        } else {
            LOGNUTPP_WARN("Invalid app setting: " << name << "=" << propval);
        }
    } else {
        LOGNUTPP_WARN("Missing app setting: " << name);
    }

    LOGNUTPP_DEBUG("Using app setting: " << name << "=" << result);
    return result;
}
} // namespace webserver
} // namespace nutpp
