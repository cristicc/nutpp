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

#ifndef NUTPP_STORAGE_USER_MODEL_H_
#define NUTPP_STORAGE_USER_MODEL_H_

#include "user.h"

#include <Wt/WFormModel.h>

namespace nutpp {
namespace storage {
/**
 * @brief Implements a data model for the User table.
 */
class UserModel : public Wt::WFormModel {
public:
    UserModel();
private:
    Wt::Dbo::QueryModel<Wt::Dbo::ptr<User>> *model_;
};
} // namespace storage
} // namespace nutpp
#endif /* NUTPP_STORAGE_USER_MODEL_H_ */
