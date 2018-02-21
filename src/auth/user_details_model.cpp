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

namespace nutpp {
namespace auth {
const Wt::WFormModel::Field UserDetailsModel::LanguageField = "language";

// C-tor.
UserDetailsModel::UserDetailsModel(LoginSession &session)
    : Wt::WFormModel(),
    session_(session)
{
    addField(LanguageField, Wt::WString::tr("nutpp.registration.lang-info"));
}

// Persistence.
void UserDetailsModel::save(const Wt::Auth::User &auth_user)
{
    Wt::Dbo::ptr<storage::User> user = session_.user(auth_user);
    user.modify()->language = storage::UserLanguage::RO; // valueText(LanguageField).toUTF8();
}
} // namespace auth
} // namespace nutpp
