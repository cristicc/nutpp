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

#include "login_session.h"

#include "storage/db_model.h"
#include "util/log.h"

#include <Wt/Auth/Dbo/AuthInfo.h>
#include <Wt/Auth/AuthService.h>
#include <Wt/Auth/FacebookService.h>
#include <Wt/Auth/GoogleService.h>
#include <Wt/Auth/HashFunction.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/Auth/PasswordStrengthValidator.h>
#include <Wt/Auth/PasswordVerifier.h>

LOGNUTPP_LOGGER_AUTH;

namespace nutpp {
namespace auth {
namespace {
Wt::Auth::AuthService nutppAuthService;
Wt::Auth::PasswordService nutppPasswordService(nutppAuthService);
std::vector<std::unique_ptr<Wt::Auth::OAuthService>> nutppOAuthServices;
} // namespace

// Configures globally the authentication services.
void LoginSession::configureAuth()
{
    nutppAuthService.setAuthTokensEnabled(true, "logincookie");
    nutppAuthService.setEmailVerificationEnabled(true);
    nutppAuthService.setEmailVerificationRequired(true);

    auto verifier = std::make_unique<Wt::Auth::PasswordVerifier>();
    verifier->addHashFunction(std::make_unique<Wt::Auth::BCryptHashFunction>(7));
    nutppPasswordService.setVerifier(std::move(verifier));
    nutppPasswordService.setAttemptThrottlingEnabled(true);
    nutppPasswordService.setStrengthValidator
        (std::make_unique<Wt::Auth::PasswordStrengthValidator>());

    if (Wt::Auth::GoogleService::configured()) {
        nutppOAuthServices.push_back(std::make_unique<Wt::Auth::GoogleService>(
                                         nutppAuthService));
    }

    if (Wt::Auth::FacebookService::configured()) {
        nutppOAuthServices.push_back(std::make_unique<Wt::Auth::FacebookService>(
                                         nutppAuthService));
    }

    for (unsigned i = 0; i < nutppOAuthServices.size(); ++i) {
        nutppOAuthServices[i]->generateRedirectEndpoint();
    }
}

void LoginSession::createDefaultAccount(const storage::DbModel &db)
{
    LOGNUTPP_INFO("Creating default user account");

    try {
        LoginSession ls(db);

        std::unique_ptr<Wt::Auth::AbstractUserDatabase::Transaction> t(
            ls.users().startTransaction());

        Wt::Auth::User user = ls.users().registerNew();
        user.setIdentity(Wt::Auth::Identity::LoginName, "admin");
        user.setEmail("admin@nutpp.local");
        LoginSession::passwordAuth().updatePassword(user, "admin");

        Wt::Dbo::ptr<storage::User> nutpp_user = ls.user(user);
        nutpp_user.modify()->role = storage::UserRole::ADMIN;
        nutpp_user.modify()->language = "en";

        if (t.get()) {
            t->commit();
        }
    } catch (const Wt::Dbo::Exception &e) {
        LOGNUTPP_ERROR("Failed to create default user: " << e.what());
    }
}

// C-tor.
LoginSession::LoginSession(const storage::DbModel &db)
{
    // FIXME: this permanently allocates a DB connection
    db.initSession(db_session_, true);
    users_ = std::make_unique<UserDatabase>(db_session_);
}

// Fetches the persisted information for the authenticated user.
Wt::Dbo::ptr<storage::User> LoginSession::user()
{
    if (login_.loggedIn()) {
        return user(login_.user());
    } else {
        return Wt::Dbo::ptr<storage::User>();
    }
}

// Fetches the persisted information for the authenticated user.
Wt::Dbo::ptr<storage::User> LoginSession::user(const Wt::Auth::User &authUser)
{
    Wt::Dbo::ptr<storage::AuthInfo> authInfo = users_->find(authUser);
    Wt::Dbo::ptr<storage::User> user = authInfo->user();

    if (!user) {
        user = db_session_.add(std::make_unique<storage::User>());
        authInfo.modify()->setUser(user);
    }

    return user;
}

// Gets the auth user db.
Wt::Auth::AbstractUserDatabase &LoginSession::users()
{
    return *users_;
}

// Gets the auth service.
const Wt::Auth::AuthService &LoginSession::auth()
{
    return nutppAuthService;
}

// Gets the passwd auth service.
const Wt::Auth::PasswordService &LoginSession::passwordAuth()
{
    return nutppPasswordService;
}

// Gets the oauth services.
const std::vector<const Wt::Auth::OAuthService *> LoginSession::oAuth()
{
    std::vector<const Wt::Auth::OAuthService *> result;
    for (auto &auth : nutppOAuthServices) {
        result.push_back(auth.get());
    }

    return result;
}
} // namespace auth
} // namespace nutpp
