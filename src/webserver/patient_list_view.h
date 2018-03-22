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
 * @brief Widget to list patients.
 */

#ifndef NUTPP_WEBSERVER_PATIENTLISTVIEW_H_
#define NUTPP_WEBSERVER_PATIENTLISTVIEW_H_

#include <Wt/WTemplateFormView.h>

namespace nutpp {
namespace webserver {
/**
 * @brief Widget to edit patient information.
 */
class PatientListView : public Wt::WTemplateFormView {
public:
    /// Constructor.
    PatientListView();

private:
    std::unique_ptr<Wt::WDialog> dialog_;
    std::unique_ptr<Wt::WMessageBox> msgbox_;
};
} // namespace webserver
} // namespace nutpp
#endif // NUTPP_WEBSERVER_PATIENTLISTVIEW_H_
