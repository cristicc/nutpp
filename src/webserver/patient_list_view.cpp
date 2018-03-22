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

#include "patient_list_view.h"

#include "patient_form_view.h"

#include <Wt/WDialog.h>
#include <Wt/WMessageBox.h>
#include <Wt/WPushButton.h>

namespace nutpp {
namespace webserver {
// C-tor.
PatientListView::PatientListView()
{
    setTemplateText(tr("nutpp.ws.template.list-patient"));

    auto but = bindWidget(
        "add-pacient-button",
        std::make_unique<Wt::WPushButton>(
            Wt::WString::tr("nutpp.patient.add-new")));

    but->clicked().connect(
        [=]() {
            dialog_ = std::make_unique<Wt::WDialog>(
                tr("nutpp.patient.add-new"));
            dialog_->contents()->addWidget(
                std::make_unique<PatientFormView>());
            // dialog_->footer()->hide();
            dialog_->contents()->childrenChanged().connect(
                [=]() { dialog_.reset(); });

            dialog_->show();
        });
}
} // namespace webserver
} // namespace nutpp
