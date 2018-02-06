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

#include "NutppUI.h"
#include "log.h"

#include <Wt/WEnvironment.h>
#include <Wt/WOverlayLoadingIndicator.h>
#include <Wt/WText.h>

#include <unistd.h>

using namespace Wt;

int NutppUI::session_cnt = 0;

//
NutppUI::NutppUI(const WEnvironment &_env)
    : WApplication(_env)
{
    if (++session_cnt > SystemController::getInstance().getMaxWebSessions()) {
        LOGCNT_WARN("Reached max allowed web sessions: "
                    << NutppUI::sessionCnt);

        // TODO: design error page
        // app->redirect("error.html");
        root()->addWidget(
            std::make_unique<Wt::WText>("ERROR: max session number reached!"));
        quit();
        return;
    }

    LOGCNT_DEBUG("Created session: " << sessionId());

    // Set theme (default, polished)
    setCssTheme("polished");

    useStyleSheet("css/nutpp_ui.css");
    // Fix for loading indicator to render properly in IE
    styleSheet().addRule("body", "margin: 0px");
    setLoadingIndicator(std::make_unique<WOverlayLoadingIndicator>());

    // Load message resources
    messageResourceBundle().use(appRoot() + "messages-main");

    // Application title
    setTitle(WString::tr("app-title"));

    // Auto update help box on context change
//    internalPathChanged().connect([=]() {
//        mainMenu_->updateHelpBox();
//    });

    // Check for JavaScript support
    if (!environment().javaScript()) {
        infoBar_->showError("JavaScript must be enabled/supported in order to"
                            " properly use the application!");
    }
}

//
NutppUI::~NutppUI()
{
    LOGCNT_DEBUG("Destroyed session: " << sessionId());

    --NutppUI::session_cnt;
}

//
void NutppUI::refresh()
{
    WApplication::refresh();
}
