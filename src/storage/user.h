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
 * @brief Persisted user information.
 */

#ifndef NUTPP_STORAGE_USER_H_
#define NUTPP_STORAGE_USER_H_

#include <string>
#include <Wt/Auth/Dbo/AuthInfo.h>
#include <Wt/Dbo/Types.h>

namespace nutpp {
namespace storage {
// Fwd declaration.
class User;

/**
 * @brief Links the Wt authentication information persistence class
 * to our custom User information persistence class.
 */
using AuthInfo = Wt::Auth::Dbo::AuthInfo<User>;

/**
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
 * @brief Class mapping the "user" database table.
 *
 * Using this class we add application specific data for a particular user
 * which is linked up with the Wt authentication system.
 */
class User {
public:
    /// The name of the DB table mapped to this class.
    static constexpr const char *kTableName = "user";

    /// Account type.
    UserRole role;

    /// User preferred language.
    std::string language;

    /// Wt standard authentication information.
    Wt::Dbo::weak_ptr<AuthInfo> auth_info;

    /**
     * @brief Defines the User class persistence.
     * @param a Database object action.
     */
    template<class Action>
    void persist(Action &a)
    {
        Wt::Dbo::field(a, role, "role");
        Wt::Dbo::field(a, language, "language");
        Wt::Dbo::hasOne(a, auth_info, kTableName);
    }
};
} // namespace storage
} // namespace nutpp
#endif // NUTPP_STORAGE_USER_H_
