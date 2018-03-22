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
 * @brief Email address input field validator.
 */

#ifndef NUTPP_WEBSERVER_EMAILVALIDATOR_H_
#define NUTPP_WEBSERVER_EMAILVALIDATOR_H_

#include <Wt/WRegExpValidator.h>

namespace nutpp {
namespace webserver {
/**
 * @brief A validator that checks user input against an email address pattern.
 */
class EmailValidator : public Wt::WRegExpValidator {
public:
    /// Constructor.
    EmailValidator();

    /// Performs the field value validation.
    Wt::WValidator::Result validate(const WT_USTRING &input) const override;
};
} // namespace webserver
} // namespace nutpp
#endif // NUTPP_WEBSERVER_EMAILVALIDATOR_H_
