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

#include "registration_view.h"

#include "login_session.h"
#include "user_details_model.h"

#include <Wt/WLineEdit.h>

namespace nutpp {
namespace auth {
// C-tor.
RegistrationView::RegistrationView(LoginSession &session,
                                   Wt::Auth::AuthWidget *authWidget)
    : Wt::Auth::RegistrationWidget(authWidget),
    session_(session)
{
    setTemplateText(tr("template.registration"));
    detailsModel_ = std::make_unique<UserDetailsModel>(session_);

    updateView(detailsModel_.get());
}

// Specialization.
std::unique_ptr<Wt::WWidget> RegistrationView::createFormWidget(
    Wt::WFormModel::Field field)
{
    if (field == UserDetailsModel::LanguageField) {
        return std::make_unique<Wt::WLineEdit>();
    } else {
        return Wt::Auth::RegistrationWidget::createFormWidget(field);
    }
}

// Specialization.
bool RegistrationView::validate()
{
    bool result = Wt::Auth::RegistrationWidget::validate();

    updateModel(detailsModel_.get());
    if (!detailsModel_->validate()) {
        result = false;
    }

    updateView(detailsModel_.get());

    return result;
}

// Specialization.
void RegistrationView::registerUserDetails(Wt::Auth::User &user)
{
    detailsModel_->save(user);
}
} // namespace auth
} // namespace nutpp
