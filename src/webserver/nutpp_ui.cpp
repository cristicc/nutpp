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

#include "nutpp_ui.h"

#include "settings.h"
#include "storage/db_model.h"
#include "util/log.h"

#include <atomic>
#include <unistd.h>

#include <Wt/WEnvironment.h>
#include <Wt/WOverlayLoadingIndicator.h>
// TODO: TEST only
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>

LOGNUTPP_LOGGER_WS;

namespace nutpp {
namespace webserver {
/**
 * @brief Hides the implementation details from the NutppUI API.
 */
class NutppUI::NutppUIImpl {
private:
    // Counter used to limit max active sessions.
    static std::atomic<int> session_cnt;

    // Grants access to all resources.
    friend class NutppUI;
};

std::atomic<int> NutppUI::NutppUIImpl::session_cnt = { 0 };

// Creates web app.
NutppUI::NutppUI(const Wt::WEnvironment &env, const storage::DbModel &db_model)
    : WApplication(env),
    impl_(std::make_unique<NutppUI::NutppUIImpl>()),
    db_model_(db_model)
{
    // Relaxed memory order constraint is fine since there is not data sync
    // involved in the usage of session_cnt.
    if (impl_->session_cnt.fetch_add(1, std::memory_order_relaxed)
        >= readAppIntSetting("maxWebSessions"))
    {
        LOGNUTPP_WARN(
            "Reached max allowed web sessions: " << impl_->session_cnt);

        // TODO: design error page
        // app->redirect("error.html");
        root()->addWidget(
            std::make_unique<Wt::WText>("ERROR: max session number reached!"));
        quit();
        return;
    }

    LOGNUTPP_DEBUG("Creating app for session: " << sessionId());

    // Set theme (default, polished)
    setCssTheme("polished");

    useStyleSheet("css/nutpp_ui.css");
    // Fix for loading indicator to render properly in IE.
    styleSheet().addRule("body", "margin: 0px");
    setLoadingIndicator(std::make_unique<Wt::WOverlayLoadingIndicator>());

    // Load message resources
    messageResourceBundle().use(appRoot() + "messages-main");

    // Application title
    setTitle(Wt::WString::tr("app-title"));

    // Auto update help box on context change
// internalPathChanged().connect([=]() {
// mainMenu_->updateHelpBox();
// });

    // TODO: Test only
    root()->addWidget(
        std::make_unique<Wt::WText>("Welcome to NUTPP"));
    auto t = root()->addWidget(std::make_unique<Wt::WTemplate>(
                                   Wt::WString::tr("test-template")));

    t->bindWidget("name-edit", std::make_unique<Wt::WLineEdit>());
    t->bindWidget("save-button", std::make_unique<Wt::WPushButton>("Save"));
    t->bindWidget("cancel-button", std::make_unique<Wt::WPushButton>("Cancel"));

    // Check for JavaScript support
    if (!environment().javaScript()) {
// infoBar_->showError("JavaScript must be enabled/supported in order to"
// " properly use the application!");
    }
}

// Destroys web app.
NutppUI::~NutppUI()
{
    LOGNUTPP_DEBUG("Destroyed app for session: " << sessionId());
    impl_->session_cnt.fetch_sub(1, std::memory_order_relaxed);
}

// Refreshes web app.
void NutppUI::refresh()
{
    WApplication::refresh();
}
} // namespace webserver
} // namespace nutpp
