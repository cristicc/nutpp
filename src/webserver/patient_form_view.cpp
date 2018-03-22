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

#include "patient_form_view.h"

#include "patient_form_model.h"

#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>

namespace nutpp {
namespace webserver {
// C-tor.
PatientFormView::PatientFormView()
    : Wt::WTemplateFormView(tr("nutpp.ws.template.edit-patient")),
    model_(std::make_unique<PatientFormModel>())
{
    setFormWidget(PatientFormModel::kNameField,
                  std::make_unique<Wt::WLineEdit>());
    setFormWidget(PatientFormModel::kEmailField,
                  std::make_unique<Wt::WLineEdit>());
    setFormWidget(PatientFormModel::kBirthDateField,
                  std::make_unique<Wt::WLineEdit>());
    setFormWidget(PatientFormModel::kGenderField,
                  std::make_unique<Wt::WLineEdit>());
    setFormWidget(PatientFormModel::kPhoneNoField,
                  std::make_unique<Wt::WLineEdit>());
    setFormWidget(PatientFormModel::kNoteField,
                  std::make_unique<Wt::WLineEdit>());

    auto save = bindWidget(
        "save-button",
        std::make_unique<Wt::WPushButton>(tr("nutpp.patient.save")));

    save->clicked().connect(
        [=]() {
            // TODO: call model save
        });

    auto cancel = bindWidget(
        "cancel-button",
        std::make_unique<Wt::WPushButton>(tr("Wt.WMessageBox.Cancel")));

    cancel->clicked().connect(
        [=]() { removeFromParent(); });

    updateView(model_.get());
}

// Destructor in .cpp required by unique_ptr to avoid incomplete type errors.
PatientFormView::~PatientFormView()
{}
} // namespace webserver
} // namespace nutpp
