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
#include "auth/auth_widget.h"
#include "auth/login_session.h"
#include "storage/db_model.h"
#include "util/log.h"

#include <atomic>
#include <unistd.h>

#include "Wt/Auth/PasswordService.h"
#include <Wt/WBootstrapTheme.h>
#include <Wt/WOverlayLoadingIndicator.h>
#include <Wt/WText.h>

LOGNUTPP_LOGGER_WS;

namespace nutpp {
namespace webserver {
/**
 * @brief Hides the implementation details from the NutppUI API.
 */
class NutppUI::NutppUIImpl {
public:
    // C-tor.
    NutppUIImpl(const NutppRuntime &runtime);

private:
    // Counter used to limit max active sessions.
    static std::atomic<int> session_cnt;
    // Reference to the shared DB model instance.
    const storage::DbModel &db_model_;
    // Handler for user authentication.
    auth::LoginSession login_session_;

    // Grants access to all resources.
    friend class NutppUI;
};

std::atomic<int> NutppUI::NutppUIImpl::session_cnt = { 0 };

// C-tor.
NutppUI::NutppUIImpl::NutppUIImpl(const NutppRuntime &runtime)
    : db_model_(runtime.db_model_),
    login_session_(runtime.db_model_)
{}

// C-tor.
NutppRuntime::NutppRuntime(
    const Wt::WEnvironment &env,
    const storage::DbModel &model)
    : env_(env),
    db_model_(model)
{}

// Creates web app.
NutppUI::NutppUI(const NutppRuntime &runtime)
    : WApplication(runtime.env_),
    impl_(std::make_unique<NutppUI::NutppUIImpl>(runtime))
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
        root()->addWidget(std::make_unique<Wt::WText>(
                              Wt::WString::tr("nutpp.ws.max-sessions-error")));
        quit();
        return;
    }

    LOGNUTPP_DEBUG("Creating app for session: " << sessionId());

    auto theme = std::make_shared<Wt::WBootstrapTheme>();
    theme->setVersion(Wt::BootstrapVersion::v3);
    setTheme(theme);

    useStyleSheet("css/nutpp-ui.css");
    root()->addStyleClass("container");
    setLoadingIndicator(std::make_unique<Wt::WOverlayLoadingIndicator>());

    // Load message resources.
    messageResourceBundle().use(appRoot() + "strings");
    messageResourceBundle().use(appRoot() + "templates");

    // Application title.
    setTitle(Wt::WString::tr("nutpp.ws.app-title"));

    auto authWidget
        = std::make_unique<nutpp::auth::AuthWidget>(impl_->login_session_);
    authWidget->model()->addPasswordAuth(&auth::LoginSession::passwordAuth());
    authWidget->model()->addOAuth(auth::LoginSession::oAuth());
    authWidget->setRegistrationEnabled(true);
    authWidget->processEnvironment();
    root()->addWidget(std::move(authWidget));

    // Handle authentication.
    impl_->login_session_.login().changed().connect([=]() {
                if (impl_->login_session_.login().loggedIn()) {
                    const Wt::Auth::User &u = impl_->login_session_.login().user();
                    LOGNUTPP_INFO(
                        "User " << u.id() << " ("
                                << u.identity(Wt::Auth::Identity::LoginName)
                                << ") logged in.");
                    // Wt::Dbo::Transaction t(session_);
                    // dbo::ptr<User> user = session_.user();
                    // log("notice") << "(Favourite pet: " << user->favouritePet
                    // << ")";
                } else {
                    LOGNUTPP_INFO("User logged out.");
                }
            });
}

// Destroys web app.
NutppUI::~NutppUI()
{
    LOGNUTPP_DEBUG("Destroyed app for session: " << sessionId());
    impl_->session_cnt.fetch_sub(1, std::memory_order_relaxed);
}

// Gets access to db model.
const storage::DbModel &NutppUI::getDbModel()
{
    return impl_->db_model_;
}

// Refreshes web app.
void NutppUI::refresh()
{
    WApplication::refresh();
}
} // namespace webserver
} // namespace nutpp
