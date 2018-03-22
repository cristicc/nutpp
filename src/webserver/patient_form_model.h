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
 * @brief Model for patient form.
 */

#ifndef NUTPP_AUTH_PATIENTFORMMODEL_H_
#define NUTPP_AUTH_PATIENTFORMMODEL_H_

#include <Wt/WFormModel.h>

namespace nutpp {
namespace webserver {

/**
 * @brief A model for PatientFormView.
 */
class PatientFormModel : public Wt::WFormModel {
public:
    /// Patient name.
    static const Field kNameField;
    /// Patient email.
    static const Field kEmailField;
    /// Patient birth date.
    static const Field kBirthDateField;
    /// Patient gender (m/f).
    static const Field kGenderField;
    /// Patient phone no.
    static const Field kPhoneNoField;
    /// Patient note.
    static const Field kNoteField;

    /// Physical activity.
    static const Field kActivityField;

    /**
     * @brief Instantiates the model.
     */
    PatientFormModel();

    /**
     * @brief Stores the user details in database.
     */
    void save();

    /**
     * @brief Gets access to language model.
     */
    std::shared_ptr<Wt::WAbstractItemModel> activityModel();

private:
    static const std::vector<std::string> kActivities;
    static const std::string &kDefaultActivity;

    std::shared_ptr<Wt::WStandardItemModel> activity_model_;
};
} // namespace auth
} // namespace nutpp
#endif // NUTPP_AUTH_PATIENTFORMMODEL_H_
