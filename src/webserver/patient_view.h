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
 * @brief Widget to view detailed patient info.
 */

#ifndef NUTPP_WEBSERVER_PATIENTVIEW_H_
#define NUTPP_WEBSERVER_PATIENTVIEW_H_

#include <Wt/WTemplate.h>
#include "storage/patient.h"

namespace nutpp {
namespace webserver {
/**
 * @brief Widget to edit patient information.
 */
class PatientView : public Wt::WTemplate {
public:
    /// Constructor.
    PatientView();

    /// Show patient.
    void showPatient(const Wt::Dbo::ptr<storage::Patient> &patient);

private:
    Wt::Dbo::ptr<storage::Patient> patient_;
};
} // namespace webserver
} // namespace nutpp
#endif // NUTPP_WEBSERVER_PATIENTVIEW_H_
