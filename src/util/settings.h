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

#ifndef NUTPP_UTIL_SETTINGS_H_
#define NUTPP_UTIL_SETTINGS_H_

#include <string>

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
 * @brief Reads a string setting from the application configuration.
 *
 * @param[in] name The name of the setting.
 * @param[in] defaultValue The value to be used if the setting is not available.
 *
 * @return The string value read.
 */
std::string readAppStringSetting(const char *name,
                                 const char *defaultValue = nullptr);

/**
 * @brief Reads an integer setting from the application configuration.
 *
 * @param[in] name The name of the setting.
 * @param[in] defaultValue The value to be used if the setting is not available.
 *
 * @return The integer value read.
 */
int readAppIntSetting(const char *name, int defaultValue = 0);

/**
 * @brief Reads a boolean setting from the application configuration.
 *
 * @param[in] name The name of the setting.
 * @param[in] defaultValue The value to be used if the setting is not available.
 *
 * @return The boolean value read.
 */
bool readAppBoolSetting(const char *name, bool defaultValue = false);
} // namespace util
} // namespace nutpp
#endif /* NUTPP_UTIL_SETTINGS_H_ */
