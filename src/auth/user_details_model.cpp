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

#include "user_details_model.h"

#include "login_session.h"
#include "storage/user.h"

#include <Wt/WStandardItemModel.h>

namespace nutpp {
namespace auth {
const Wt::WFormModel::Field UserDetailsModel::kLanguageField = "language";

const std::vector<std::string> UserDetailsModel::kLanguages = {
    "en", "ro", "de"
};
const std::string &UserDetailsModel::kDefaultLanguage
    = UserDetailsModel::kLanguages[0];

// C-tor.
UserDetailsModel::UserDetailsModel(LoginSession &session)
    : Wt::WFormModel(),
    session_(session)
{
    // Initialize language model.
    language_model_
        = std::make_shared<Wt::WStandardItemModel>(kLanguages.size(), 1);

    for (size_t i = 0; i < kLanguages.size(); ++i) {
        language_model_->setData(i, 0, kLanguages[i], Wt::ItemDataRole::User);
        language_model_->setData(
            i, 0, Wt::WString::tr("nutpp.lang." + kLanguages[i]),
            Wt::ItemDataRole::Display);
    }

    // Add fields.
    addField(kLanguageField, Wt::WString::tr("nutpp.auth.language-info"));
    setValue(kLanguageField, kDefaultLanguage);
}

// Persistence.
void UserDetailsModel::save(const Wt::Auth::User &auth_user)
{
    Wt::Dbo::ptr<storage::User> user = session_.user(auth_user);
    user.modify()->role = storage::UserRole::REGULAR;
    user.modify()->language = valueText(kLanguageField).toUTF8();
}

// Getter.
std::shared_ptr<Wt::WAbstractItemModel> UserDetailsModel::languageModel()
{
    return language_model_;
}
} // namespace auth
} // namespace nutpp
