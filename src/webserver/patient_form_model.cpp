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

#include "patient_form_model.h"

#include "email_validator.h"
#include "nutpp_ui.h"
#include "auth/login_session.h"
#include "storage/db_session.h"
#include "util/log.h"

#include <Wt/WDateValidator.h>
#include <Wt/WLengthValidator.h>
#include <Wt/WRegExpValidator.h>
#include <Wt/WStandardItemModel.h>

LOGNUTPP_LOGGER_WS;

namespace nutpp {
namespace webserver {
namespace {
// Name validator.
std::shared_ptr<Wt::WValidator> createNameValidator()
{
    auto v = std::make_shared<Wt::WLengthValidator>(2, 50);
    v->setMandatory(true);
    return v;
}

// Email validator.
std::shared_ptr<Wt::WValidator> createEmailValidator()
{
    return std::make_shared<EmailValidator>();
}

// Birth date validator.
std::shared_ptr<Wt::WValidator> createBirthDateValidator()
{
    auto v = std::make_shared<Wt::WDateValidator>(
        "dd/MM/yyyy", Wt::WDate(1900, 1, 1), Wt::WDate::currentDate());
    v->setMandatory(true);
    return v;
}

// Gender validator.
std::shared_ptr<Wt::WValidator> createGenderValidator()
{
    return std::make_shared<Wt::WValidator>(true);
}

// Phone validator.
std::shared_ptr<Wt::WValidator> createPhoneNoValidator()
{
    auto v = std::make_shared<Wt::WRegExpValidator>("[0-9+(). ext-]{3,20}");
    v->setInvalidNoMatchText(Wt::WString::tr("nutpp.patient.phone-invalid"));
    v->setMandatory(true);
    return v;
}

// Note validator.
std::shared_ptr<Wt::WValidator> createNoteValidator()
{
    return std::make_shared<Wt::WLengthValidator>(0, 1024);
}
} // namespace

const Wt::WFormModel::Field PatientFormModel::kNameField = "name";
const Wt::WFormModel::Field PatientFormModel::kEmailField = "email";
const Wt::WFormModel::Field PatientFormModel::kBirthDateField = "birth-date";
const Wt::WFormModel::Field PatientFormModel::kGenderField = "gender";
const Wt::WFormModel::Field PatientFormModel::kPhoneNoField = "phone";
const Wt::WFormModel::Field PatientFormModel::kNoteField = "note";

const Wt::WFormModel::Field PatientFormModel::kActivityField = "activity";

const std::vector<std::string> PatientFormModel::kActivities = {
    { "office", "sport", "home" }
};
const std::string &PatientFormModel::kDefaultActivity
    = PatientFormModel::kActivities[0];

// C-tor.
PatientFormModel::PatientFormModel(const Wt::Dbo::ptr<storage::Patient> &patient)
    : Wt::WFormModel(),
    patient_(patient)
{
    initializeModels();

    // Add model fields.
    addField(kNameField, Wt::WString::tr("nutpp.patient.name-info"));
    addField(kEmailField, Wt::WString::tr("nutpp.patient.email-info"));
    addField(kBirthDateField, Wt::WString::tr("nutpp.patient.birth-date-info"));
    addField(kGenderField, Wt::WString::tr("nutpp.patient.gender-info"));
    addField(kPhoneNoField, Wt::WString::tr("nutpp.patient.phone-info"));
    addField(kNoteField, Wt::WString::tr("nutpp.patient.note-info"));

    // TODO: move to patient progress model
    addField(kActivityField, Wt::WString::tr("nutpp.auth.activity-info"));

    // Add validators.
    setValidator(kNameField, createNameValidator());
    setValidator(kEmailField, createEmailValidator());
    setValidator(kBirthDateField, createBirthDateValidator());
    setValidator(kGenderField, createGenderValidator());
    setValidator(kPhoneNoField, createPhoneNoValidator());
    setValidator(kNoteField, createNoteValidator());

    if (patient_.isTransient()) {
        // New patient, use default values.
        setValue(kBirthDateField, Wt::WDate());
        setValue(kGenderField, std::string(""));
        setValue(kActivityField, kDefaultActivity);
    } else {
        // Load patient data from DB.
        try {
            storage::DbSession s(patient_);
            setValue(kNameField, patient->name);
            setValue(kEmailField, patient->email);
            setValue(kBirthDateField, patient->birth_date);
            setValue(kGenderField, patient->gender);
            setValue(kPhoneNoField, patient->phone_no);
            setValue(kNoteField, patient->note);
        } catch (const std::exception &e) {
            LOGNUTPP_ERROR("Failed to access patient data: " << e.what());
        }
    }
}

// Persistence.
bool PatientFormModel::save()
{
    try {
        auto &s = NUTPP_LOGIN.dbSession();
        auto t = s.createTransaction();

        if (patient_.isTransient()) {
            // Adding new patient.
            s.add(patient_);
        }

        auto patient = patient_.modify();
        patient->name = valueText(kNameField).toUTF8();
        patient->email = valueText(kEmailField).toUTF8();
        patient->birth_date
            = Wt::cpp17::any_cast<Wt::WDate>(value(kBirthDateField));
        patient->gender = valueText(kGenderField).toUTF8();
        patient->phone_no = valueText(kPhoneNoField).toUTF8();
        patient->note = valueText(kNoteField).toUTF8();
        patient->modif_time = Wt::WDateTime::currentDateTime();
        patient->owner = NUTPP_LOGIN.user();

        return t->commit();
    } catch (const std::exception &e) {
        LOGNUTPP_ERROR("Failed to save patient: " << e.what());
    }

    return false;
}

// Getter.
std::shared_ptr<Wt::WAbstractItemModel> PatientFormModel::genderModel()
{
    return gender_model_;
}

// Getter.
std::shared_ptr<Wt::WAbstractItemModel> PatientFormModel::activityModel()
{
    return activity_model_;
}

// Init models.
void PatientFormModel::initializeModels()
{
    // Gender model.
    static const std::vector<std::string> genders = { "", "m", "f" };

    gender_model_
        = std::make_shared<Wt::WStandardItemModel>(genders.size(), 1);

    for (size_t i = 0; i < genders.size(); ++i) {
        gender_model_->setData(i, 0, genders[i], Wt::ItemDataRole::User);
        gender_model_->setData(
            i, 0,
            Wt::WString::tr("nutpp.patient.gender."
                            + (i > 0 ? genders[i] : "n")),
            Wt::ItemDataRole::Display);
    }

    // Activity model.
    activity_model_
        = std::make_shared<Wt::WStandardItemModel>(kActivities.size(), 1);

    for (size_t i = 0; i < kActivities.size(); ++i) {
        activity_model_->setData(i, 0, kActivities[i], Wt::ItemDataRole::User);
        activity_model_->setData(
            i, 0, Wt::WString::tr("nutpp.lang." + kActivities[i]),
            Wt::ItemDataRole::Display);
    }
}
} // namespace auth
} // namespace nutpp
