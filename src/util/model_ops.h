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
 * @brief General purpose model utilities.
 */

#ifndef NUTPP_UTIL_MODELOPS_H_
#define NUTPP_UTIL_MODELOPS_H_

#include <string>
#include <Wt/WStandardItemModel.h>

namespace nutpp {
namespace util {
/**
 * @brief Identifies the index of a form field value in the associated
 * item model, the value being taken from a form model.
 *
 * @param[in] item_name The name of the form field.
 * @param[in] item_model The model associated to the form field identified
 *            by @p item_name.
 * @param[in] form_model The model associated to the form containing
 *            the field @p item_name.
 *
 * @return The index in the @p item_model corresponding to the current value
 * the item has in the @p form_model. If the value was not found,
 * the return value is @c (since the first item is usually the default
 * selected).
 */
template<typename ValueType>
int getItemModelCurrentIndex(
    Wt::WFormModel::Field item_name, const Wt::WAbstractItemModel &item_model,
    const Wt::WFormModel &form_model)
{
    ValueType val = Wt::cpp17::any_cast<ValueType>(form_model.value(item_name));

    for (int i = 0; i < item_model.rowCount(); ++i) {
        if (Wt::cpp17::any_cast<ValueType>(
                item_model.data(i, 0, Wt::ItemDataRole::User)) == val)
        {
            return i;
        }
    }

    return 0;    // return first item by default
}

/**
 * @brief Fetches the value of a form field that corresponds to a given
 * index in the item model.
 *
 * @param[in] index The index of the form field value in associated model.
 * @param[in] item_model The model associated to the form field.
 * @return The value of the form field at the specified index.
 */
template<typename ValueType>
ValueType getItemModelValueAt(
    int index, const Wt::WAbstractItemModel &item_model)
{
    return Wt::cpp17::any_cast<ValueType>(
        item_model.data((index < 0 ? 0 : index), 0, Wt::ItemDataRole::User));
}
} // namespace util
} // namespace nutpp
#endif // NUTPP_UTIL_MODELOPS_H_
