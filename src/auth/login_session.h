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
 * @brief Authenticated session model.
 */

#ifndef NUTPP_AUTH_LOGINSESSION_H_
#define NUTPP_AUTH_LOGINSESSION_H_

#include "storage/user.h"

#include <Wt/Auth/Dbo/UserDatabase.h>
#include <Wt/Auth/Login.h>
#include <Wt/Dbo/ptr.h>

namespace nutpp {
namespace storage {
class DbModel;
class DbSession;
} // namespace storage

namespace auth {
/**
 * @brief Type alias for UserDatabase, indicating the usage of
 * Wt::Auth::Dbo::UserDatabase implementation using storage::AuthInfo.
 */
using UserDatabase = Wt::Auth::Dbo::UserDatabase<storage::AuthInfo>;

/**
 * @brief Model for user authentication in web sessions.
 *
 * Implements a model that encapsulates the connection to the database
 * to store authentication information. Additionally, tracks the user
 * currently logged in a web session.
 *
 * The following features are provided by the Wt authentication module,
 * which can be separately enabled, configured or customized:
 * <ul>
 * <li><b>Password authentication</b><br>
 *     Includes salted hashing with strong cryptographic hash functions
 *     (e.g. @e bcrypt) and password strength checking.
 * <li><b>Remember-me functionality</b><br>
 *     Associates authentication tokens stored in cookies to a user.
 * <li><b>Verified email addresses</b><br>
 *     Uses the typical confirmation email process.
 * <li><b>Lost password recovery</b><br>
 *     Uses the verified email address to prompt a user to enter a new password.
 * <li><b>Authentication using 3rd party Identity Providers</b><br>
 *     Uses @e OAuth2, with support for multiple identities per user.
 *     Currently, only Google and Facebook are supported for authentication,
 * <li><b>Registration logic</b><br>
 *     Includes also the logic needed to merge new (federated login) identities
 *     into existing user profiles. For example, if a user previously
 *     registered using a user name and password, he may later also authenticate
 *     using for example his Google Account and this new identity is added to
 *     his existing account.
 * </ul>
 */
class LoginSession {
public:
    /**
     * @brief Configures globally the authentication services.
     *
     * The application configuration file allows using Google and/or Facebook
     * as an authentication provider, assuming the application has been
     * registered with Google and/or Facebook.
     *
     * @warning To be called once, before the first instantiation of the class.
     */
    static void configureAuth();

    /**
     * @brief Creates a default login user with admin privileges.
     *
     * The account has the name @e admin and the login password is @e admin.
     */
    static void createDefaultAccount(const storage::DbModel &db);

    /// Gets access to the auth service.
    static const Wt::Auth::AuthService &auth();

    /// Gets access to the password auth service.
    static const Wt::Auth::PasswordService &passwordAuth();

    /// Gets access to the OAuth services.
    static const std::vector<const Wt::Auth::OAuthService *> oAuth();

    /**
     * @brief Instantiates the class using the given data model.
     * @param[in] db The data model that holds the application specific user
     * details and the connection pool.
     */
    LoginSession(const storage::DbModel &db);

    /// Destructor.
    ~LoginSession();

    /**
     * @brief Gets access to the authentication user database.
     * @return A reference to the authentication user database.
     */
    Wt::Auth::AbstractUserDatabase &users();

    /// Fetches the persisted details of the authenticated user.
    Wt::Dbo::ptr<storage::User> user();

    /// Fetches the persisted details for a user.
    Wt::Dbo::ptr<storage::User> user(const Wt::Auth::User &user);

    /// Gets access to the login state manager.
    Wt::Auth::Login &login() { return login_; }

    /// Gets access to the database session.
    storage::DbSession &dbSession() { return *db_session_; }

private:
    std::unique_ptr<storage::DbSession> db_session_;
    std::unique_ptr<UserDatabase> users_;
    Wt::Auth::Login login_;
};
} // namespace auth
} // namespace nutpp
#endif // NUTPP_AUTH_LOGINSESSION_H_
