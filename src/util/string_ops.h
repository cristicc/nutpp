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
 * @brief General purpose utilities.
 */

#ifndef NUTPP_UTIL_STRINGOPS_H_
#define NUTPP_UTIL_STRINGOPS_H_

#include <string>

namespace nutpp {
namespace util {
/**
 * @brief Replaces all occurrences of a string with another string.
 *
 * @param[in] context The text to work on.
 * @param[in] from The string to be replaced.
 * @param[in] to The string to be used as replacement.
 *
 * @return The resulting string.
 */
std::string replaceAll(const std::string &context,
                       const std::string &from,
                       const std::string &to);

/**
 * @brief Performs a transformation of a relative file path into
 * an absolute one.
 *
 * If the given \a path is a relative one, the prefix @p root is added
 * to form an absolute path, otherwise no operation is performed.
 *
 * @param[in] path The file path to be processed.
 * @param[in] root The prefix to be added to relative paths.
 *
 * @return The transformed path.
 */
std::string sanitizeFilePath(const std::string &path,
                             const std::string &root);
} // namespace util
} // namespace nutpp
#endif // NUTPP_UTIL_STRINGOPS_H_
