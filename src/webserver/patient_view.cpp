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

#include "patient_view.h"

#include "nutpp_ui.h"
#include "auth/login_session.h"
#include "storage/db_session.h"
#include "util/log.h"

LOGNUTPP_LOGGER_WS;

namespace nutpp {
namespace webserver {
/**
 * @brief Panel widget to display patient profile info.
 */
class PatientProfilePanel : public Wt::WTemplate {
public:
    /// C-tor.
    PatientProfilePanel(const Wt::Dbo::ptr<storage::Patient> &patient);
};

// C-tor.
PatientProfilePanel::PatientProfilePanel(
    const Wt::Dbo::ptr<storage::Patient> &patient)
{
    addFunction("tr", &WTemplate::Functions::tr);
    setTemplateText(tr("nutpp.ws.template.patient-profile-panel"));

    try {
        auto t = NUTPP_LOGIN.dbSession().createTransaction();

        bindEmpty("profile-pic");
        bindString("name", patient->name);
        bindString("email", patient->email);
        bindEmpty("contents");

    } catch (const std::exception &e) {
        LOGNUTPP_ERROR("Failed to access patient profile: " << e.what());
    }
}

/**
 * @brief Panel widget to display nutrition plans.
 */
class NutritionPlansPanel : public Wt::WTemplate {
public:
    /// C-tor.
    NutritionPlansPanel(const Wt::Dbo::ptr<storage::Patient> &patient);
};

// C-tor.
NutritionPlansPanel::NutritionPlansPanel(
    const Wt::Dbo::ptr<storage::Patient> &patient)
{
    addFunction("tr", &WTemplate::Functions::tr);
    setTemplateText(tr("nutpp.ws.template.nutrition-plans-panel"));

    try {
        auto t = NUTPP_LOGIN.dbSession().createTransaction();

        bindEmpty("contents");

    } catch (const std::exception &e) {
        LOGNUTPP_ERROR("Failed to access nutrition plans: " << e.what());
    }
}

/**
 * @brief Panel widget to display progress indicators.
 */
class ProgressIndicatorsPanel : public Wt::WTemplate {
public:
    /// C-tor.
    ProgressIndicatorsPanel(const Wt::Dbo::ptr<storage::Patient> &patient);
};

// C-tor.
ProgressIndicatorsPanel::ProgressIndicatorsPanel(
    const Wt::Dbo::ptr<storage::Patient> &patient)
{
    addFunction("tr", &WTemplate::Functions::tr);
    setTemplateText(tr("nutpp.ws.template.progress-indicators-panel"));

    try {
        auto t = NUTPP_LOGIN.dbSession().createTransaction();

        bindEmpty("contents");

    } catch (const std::exception &e) {
        LOGNUTPP_ERROR("Failed to access progress indicators: " << e.what());
    }
}

// C-tor.
PatientView::PatientView()
{
    addFunction("tr", &WTemplate::Functions::tr);
    setTemplateText(tr("nutpp.ws.template.view-patient"));
}

// Display info for the given patient.
void PatientView::showPatient(const Wt::Dbo::ptr<storage::Patient> &patient)
{
    patient_ = patient;

    bindWidget("patient-profile-panel",
               std::make_unique<PatientProfilePanel>(patient));
    bindWidget("nutrition-plans-panel",
               std::make_unique<NutritionPlansPanel>(patient));
    bindWidget("progress-indicators-panel",
               std::make_unique<ProgressIndicatorsPanel>(patient));
    bindEmpty("contents");
}
} // namespace webserver
} // namespace nutpp
