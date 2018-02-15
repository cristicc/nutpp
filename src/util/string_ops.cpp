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

#include "util/string_ops.h"

namespace nutpp {
namespace util {
// Text replacement utility.
std::string replaceAll(const std::string &context,
                       const std::string &from,
                       const std::string &to)
{
    std::size_t lookHere = 0;
    std::size_t foundHere;

    std::string result = context;
    while ((foundHere = result.find(from, lookHere)) != std::string::npos) {
        result.replace(foundHere, from.size(), to);
        lookHere = foundHere + to.size();
    }

    return result;
}

// Transforms a relative file path into an absolute one.
std::string sanitizeFilePath(const std::string &path, const std::string &root)
{
    if (!path.empty() && path[0] != '/') {
        return root + "/" + path;
    }

    return path;
}
} // namespace util
} // namespace nutpp
