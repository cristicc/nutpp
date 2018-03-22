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
 * @brief Persisted patient progress indicators.
 */

#ifndef NUTPP_STORAGE_PATIENTPROGRESS_H_
#define NUTPP_STORAGE_PATIENTPROGRESS_H_

#include <Wt/Dbo/Types.h>

namespace nutpp {
namespace storage {
// Fwd decl.
class Patient;

/**
 * @brief Class mapping the "patient_progress" database table.
 */
class PatientProgress {
public:
    /// The name of the DB table mapped to this class.
    static constexpr const char *kTableName = "patient_progress";

    /// Patient mass (kg).
    short mass;
    /// Patient height (cm).
    short height;
    /// Abdominal circumference (cm).
    short circum;
    /// Physical activity.
    std::string activity;
    /// Profile image.
    //std::vector<byte> image;

    /// Owned by a patient.
    Wt::Dbo::ptr<Patient> patient;

    /**
     * @brief Defines the PatientProgress class persistence.
     * @param a Database object action.
     */
    template<class Action>
    void persist(Action &a)
    {
        Wt::Dbo::field(a, mass, "mass");
        Wt::Dbo::field(a, height, "height");
        Wt::Dbo::field(a, circum, "circum");
        Wt::Dbo::field(a, activity, "activity");
        //Wt::Dbo::field(a, image, "image");
        Wt::Dbo::belongsTo(a, patient, "patient");
    }
};
} // namespace storage
} // namespace nutpp
#endif // NUTPP_STORAGE_PATIENTPROGRESS_H_
