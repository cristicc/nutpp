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
#include "util/model_ops.h"

#include <Wt/WComboBox.h>
#include <Wt/WLineEdit.h>

namespace nutpp {
namespace auth {
// C-tor.
RegistrationView::RegistrationView(LoginSession &session,
                                   Wt::Auth::AuthWidget *auth_widget)
    : Wt::Auth::RegistrationWidget(auth_widget)
{
    setTemplateText(tr("nutpp.auth.template.registration"));
    details_model_ = std::make_unique<UserDetailsModel>(session);

    auto combo = std::make_unique<Wt::WComboBox>();
    auto combo_ptr = combo.get();

    combo->setModel(details_model_->languageModel());

    setFormWidget(
        UserDetailsModel::LanguageField,
        std::move(combo),
        [=] { // updateViewValue()
            combo_ptr->setCurrentIndex(
                util::getItemModelCurrentIndex<std::string>(
                    UserDetailsModel::LanguageField,
                    *details_model_->languageModel(),
                    *details_model_));
        },
        [=] { // updateModelValue()
            details_model_->setValue(
                UserDetailsModel::LanguageField,
                util::getItemModelValueAt<std::string>(
                    combo_ptr->currentIndex(),
                    *details_model_->languageModel()));
        });

    updateView(details_model_.get());
}

// Destructor in .cpp required by unique_ptr to avoid incomplete type errors.
RegistrationView::~RegistrationView() = default;

// Specialization.
bool RegistrationView::validate()
{
    bool result = Wt::Auth::RegistrationWidget::validate();

    updateModel(details_model_.get());
    if (!details_model_->validate()) {
        result = false;
    }

    updateView(details_model_.get());

    return result;
}

// Specialization.
void RegistrationView::registerUserDetails(Wt::Auth::User &user)
{
    details_model_->save(user);
}
} // namespace auth
} // namespace nutpp
