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
 * @brief Custom authentication widget.
 */

#ifndef NUTPP_AUTH_AUTHWIDGET_H_
#define NUTPP_AUTH_AUTHWIDGET_H_

#include <Wt/Auth/AuthWidget.h>

namespace nutpp {
namespace auth {
// Fwd declaration.
class LoginSession;

/**
 * @brief A custom authentication widget.
 *
 * Extends the Wt's @c AuthWidget to provide a custom registration view.
 * It uses a LoginSession instance for managing the user authentication.
 *
 * @sa RegistrationView
 * @sa UserDetailsModel
 */
class AuthWidget : public Wt::Auth::AuthWidget {
public:
    /**
     * @brief Creates a class instance.
     * @param[in] A reference to a LoginSession instance.
     */
    AuthWidget(LoginSession &session);

    /**
     * @brief Creates a custom registration widget using
     * RegistrationView and UserDetailsModel.
     */
    virtual std::unique_ptr<Wt::WWidget> createRegistrationView(
        const Wt::Auth::Identity &id) override;

private:
    LoginSession &session_;
};
} // namespace storage
} // namespace nutpp
#endif // NUTPP_AUTH_AUTHWIDGET_H_
