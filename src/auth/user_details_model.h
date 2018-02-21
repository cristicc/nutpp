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
 * @brief Model for custom user details.
 */

#ifndef NUTPP_AUTH_USERDETAILSMODEL_H_
#define NUTPP_AUTH_USERDETAILSMODEL_H_

#include <Wt/WFormModel.h>

namespace nutpp {
namespace auth {
// Fwd declaration.
class LoginSession;

/**
 * @brief A model to handle custom user details.
 */
class UserDetailsModel : public Wt::WFormModel {
public:
    /// User language.
    static const Field LanguageField;

    /**
     * @brief Instantiates the model.
     * @param[in] A reference to a LoginSession instance.
     */
    UserDetailsModel(LoginSession &session);

    /**
     * @brief Stores the user details.
     * @param[in] auth_user The authenticated user.
     */
    void save(const Wt::Auth::User &auth_user);

private:
    LoginSession &session_;
};
} // namespace auth
} // namespace nutpp
#endif // NUTPP_AUTH_USERDETAILSMODEL_H_
