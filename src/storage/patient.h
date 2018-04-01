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
 * @brief Persisted patient information.
 */

#ifndef NUTPP_STORAGE_PATIENT_H_
#define NUTPP_STORAGE_PATIENT_H_

#include "patient_progress.h"

#include <Wt/WDate.h>

namespace nutpp {
namespace storage {
// Fwd decl.
class User;
/**
 * @brief Class mapping the "patient" database table.
 */
class Patient {
public:
    /// The name of the DB table mapped to this class.
    static constexpr const char *kTableName = "patient";

    /// Patient name.
    std::string name;
    /// Patient email.
    std::string email;
    /// Patient birth date.
    Wt::WDate birth_date;
    /// Patient gender (m/f).
    std::string gender;
    /// Patient phone no.
    std::string phone_no;
    /// Some info.
    std::string note;
    /// Profile image.
    //TODO: blob for profile_image in a separate table shared with User
    /// Date of last changes.
    Wt::WDateTime modif_time;

    /// Owns zero or more progress indicators.
    Wt::Dbo::collection<Wt::Dbo::ptr<PatientProgress>> progress_inds;

    /// Owned by a user account.
    Wt::Dbo::ptr<User> owner;

    /**
     * @brief Defines the Patient class persistence.
     * @param a Database object action.
     */
    template<class Action>
    void persist(Action &a)
    {
        Wt::Dbo::field(a, name, "name");
        Wt::Dbo::field(a, email, "email");
        Wt::Dbo::field(a, birth_date, "birth_date");
        Wt::Dbo::field(a, gender, "gender");
        Wt::Dbo::field(a, phone_no, "phone_no");
        Wt::Dbo::field(a, note, "note");
        Wt::Dbo::field(a, modif_time, "modif_time");
        Wt::Dbo::hasMany(a, progress_inds, Wt::Dbo::ManyToOne, kTableName);
        Wt::Dbo::belongsTo(a, owner, "owner");
    }
};
} // namespace storage
} // namespace nutpp
#endif // NUTPP_STORAGE_PATIENT_H_
