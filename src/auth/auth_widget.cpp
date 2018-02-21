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

#include "auth_widget.h"

#include "login_session.h"
#include "registration_view.h"
#include "user_details_model.h"

namespace nutpp {
namespace auth {
// C-tor.
AuthWidget::AuthWidget(LoginSession &session)
    : Wt::Auth::AuthWidget(
        LoginSession::auth(), session.users(), session.login()),
    session_(session)
{}

// Creates registration view.
std::unique_ptr<Wt::WWidget> AuthWidget::createRegistrationView(
    const Wt::Auth::Identity &id)
{
    auto registrationView
        = std::make_unique<RegistrationView>(session_, this);
    std::unique_ptr<Wt::Auth::RegistrationModel> model
        = createRegistrationModel();

    if (id.isValid()) {
        model->registerIdentified(id);
    }

    registrationView->setModel(std::move(model));
    return std::move(registrationView);
}
} // namespace storage
} // namespace nutpp
