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

#ifndef NUTPP_UTIL_STRINGOPS_H_
#define NUTPP_UTIL_STRINGOPS_H_

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
 * @brief Performs an inline replacement of all occurrences of a string
 * with another string.
 *
 * @param[in,out] context The text to work on.
 * @param[in] from The string to be replaced.
 * @param[in] to The string to be used as replacement.
 *
 * @return The resulted text, which is actually the @c context.
 */
std::string &replaceAll(std::string &context,
                        const std::string &from,
                        const std::string &to);
} // namespace util
} // namespace nutpp
#endif /* NUTPP_UTIL_STRINGOPS_H_ */
