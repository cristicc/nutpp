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
 * @brief Custom user registration widget.
 */

#ifndef NUTPP_AUTH_REGISTRATIONVIEW_H_
#define NUTPP_AUTH_REGISTRATIONVIEW_H_

#include <Wt/Auth/RegistrationWidget.h>

namespace nutpp {
namespace auth {
// Fwd declaration.
class LoginSession;
// Fwd declaration.
class UserDetailsModel;

/**
 * @brief Implements a custom user registration page.
 */
class RegistrationView : public Wt::Auth::RegistrationWidget {
public:
    /**
     * @brief Creates a class instance.
     * @param[in] session The user authentication manager for current session.
     */
    RegistrationView(LoginSession &session,
                     Wt::Auth::AuthWidget *auth_widget = nullptr);

protected:
    /*
     * @brief Specialization to also validate the user details.
     */
    virtual bool validate() override;

    /**
     * @brief Specialization to register user details.
     */
    virtual void registerUserDetails(Wt::Auth::User &user) override;

private:
    LoginSession &session_;
    std::unique_ptr<UserDetailsModel> details_model_;
};
} // namespace auth
} // namespace nutpp
#endif // NUTPP_AUTH_REGISTRATIONVIEW_H_
