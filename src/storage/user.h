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

#ifndef NUTPP_STORAGE_USER_H_
#define NUTPP_STORAGE_USER_H_

#include <Wt/Dbo/Dbo.h>
#include <string>

namespace nutpp {
namespace storage {
/**
 * @enum UserRole
 * @brief Enum class representing the types of user accounts recognized
 * by the system.
 */
enum class UserRole {
    /// Guest account.
    GUEST = 0,
    /// Administrator account.
    ADMIN,
    /// Normal, non-privileged account.
    REGULAR,
    /// Reserved for unsupported accounts.
    UNKNOWN = 100
};

/**
 * @class User
 * @brief Class mapping the "user" database table.
 */
class User {
public:
    /// Full user name.
    std::string name;
    /// Email address (also used for account identification).
    std::string email;
    /// Account password.
    std::string passwd;
    /// Account type.
    UserRole role;

    /**
     * @brief Defines the User class persistence.
     *
     * @param a Database object action.
     */
    template<class Action>
    void persist(Action &a)
    {
        Wt::Dbo::field(a, name, "name");
        Wt::Dbo::field(a, passwd, "passwd");
        Wt::Dbo::field(a, role, "role");
    }
};
} // namespace storage
} // namespace nutpp
#endif /* NUTPP_STORAGE_USER_H_ */
