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

#include <Wt/Auth/PasswordService.h>
#include <Wt/WAnchor.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WLineEdit.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WOverlayLoadingIndicator.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WText.h>

#include <Wt/Http/Client.h>

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
    // Logged in user identity.
    WT_USTRING auth_identity_;
    // Reference to the authentication widget.
    auth::AuthWidget *auth_widget_ = nullptr;
    // Reference to the main navigation bar.
    Wt::WNavigationBar *nav_bar_ = nullptr;
    // Popup widget for user account details.
    std::unique_ptr<Wt::WPopupWidget> user_account_popup_;

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

    auto auth_widget
        = std::make_unique<auth::AuthWidget>(impl_->login_session_);
    auth_widget->model()->addPasswordAuth(&auth::LoginSession::passwordAuth());
    auth_widget->model()->addOAuth(auth::LoginSession::oAuth());
    auth_widget->setRegistrationEnabled(true);
    auth_widget->processEnvironment();
    impl_->auth_widget_ = root()->addWidget(std::move(auth_widget));

    // Handle user login/logout.
    impl_->login_session_.login().changed().connect([=]() {
//FIXME: test done()
//        auto client = addChild(std::make_unique<Wt::Http::Client>());
//        client->setTimeout(std::chrono::seconds{15});
//        client->setMaximumResponseSize(10 * 1024);
//        client->done().connect([=](Wt::AsioWrapper::error_code err, const Wt::Http::Message& response) { LOGNUTPP_WARN("test ok"); });
//        client->get("http://www.webtoolkit.eu/wt/blog/feed/");
//        sleep(5);

        if (impl_->login_session_.login().loggedIn()) {
            handleLoggedIn(false);
        } else {
            auto msg_box = addChild(
                std::make_unique<Wt::WMessageBox>(
                    Wt::WString::tr("nutpp.auth.sign-out"),
                    Wt::WString::tr("nutpp.auth.sign-out-message"),
                    Wt::Icon::Information, Wt::StandardButton::Ok));

            msg_box->buttonClicked().connect([=] {
                LOGNUTPP_INFO("User " << impl_->auth_identity_ << " logged out");
                removeChild(msg_box);
                redirect("");
                quit();
            });

            msg_box->show();
        }
    });

    // Handle automatic login (remember me).
    if (impl_->login_session_.login().loggedIn()) {
        handleLoggedIn(true);
    }
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

// Handles logged in.
void NutppUI::handleLoggedIn(bool automatic)
{
    const Wt::Auth::User &u = impl_->login_session_.login().user();
    impl_->auth_identity_ = u.identity(Wt::Auth::Identity::LoginName);

    const char *mode = automatic ? "automatically" : "manually";
    LOGNUTPP_INFO("User " << impl_->auth_identity_ << " logged in " << mode);

    // Mitigate session ID fixation attacks, done by default on manual login.
    if (automatic) {
        changeSessionId();
    }

    // Create main page.
    createNavBar();
}

// Create resources.
void NutppUI::createNavBar()
{
    impl_->nav_bar_
        = root()->addWidget(std::make_unique<Wt::WNavigationBar>());
    impl_->nav_bar_->setResponsive(true);
    // Workaround for missing a setImage() method in WNavigationBar API.
    impl_->nav_bar_->setTitle("");
    auto title_link = impl_->nav_bar_->resolve<Wt::WAnchor *>("title-link");
    title_link->setImage(std::make_unique<Wt::WImage>("favicon.ico"));

    // Setup a Left-aligned menu.
    Wt::WStackedWidget *contents_stack_left
        = root()->addWidget(std::make_unique<Wt::WStackedWidget>());

    auto left_menu = impl_->nav_bar_->addMenu(
        std::make_unique<Wt::WMenu>(contents_stack_left));

    left_menu->addItem(Wt::WString::tr("nutpp.nav.patients"),
                       std::make_unique<Wt::WText>("Lista pacienti.."))
    ->setLink(Wt::WLink(Wt::LinkType::InternalPath, "/patients"));

    auto search_result_tmp
        = std::make_unique<Wt::WText>("Lista formulare..");
    auto search_result = search_result_tmp.get();

    left_menu->addItem(Wt::WString::tr("nutpp.nav.forms"),
                       std::move(search_result_tmp));

    // Setup a Right-aligned menu.
    auto right_menu = impl_->nav_bar_->addMenu(
        std::make_unique<Wt::WMenu>(), Wt::AlignmentFlag::Right);

    // Create a popup submenu for the Help menu.
    auto popup = std::make_unique<Wt::WPopupMenu>();
    popup->addItem("Contents");
    popup->addItem("Index");
    popup->addSeparator();
    popup->addItem("About");

    auto popup_tmp = popup.get();
    popup->itemSelected().connect([=](Wt::WMenuItem *item) {
        auto msg_box = popup_tmp->addChild(
            std::make_unique<Wt::WMessageBox>(
                "Help",
                Wt::WString("<p>Showing Help: {1}</p>").arg(item->text()),
                Wt::Icon::Information, Wt::StandardButton::Ok));

        msg_box->buttonClicked().connect([=] {
            popup_tmp->removeChild(msg_box);
        });

        msg_box->show();
    });

    right_menu->addItem(Wt::WString::tr("nutpp.nav.help"))
        ->setMenu(std::move(popup));

    // Add a Search control.
    auto edit = std::make_unique<Wt::WLineEdit>();
    edit->setPlaceholderText(Wt::WString::tr("nutpp.nav.search-hint"));

    auto edit_tmp = edit.get();
    edit->enterPressed().connect([=] {
        left_menu->select(0); // is the index of the "Patients"
        search_result->setText(
            Wt::WString::tr("nutpp.nav.search-not-found").arg(edit_tmp->text()));
    });

    impl_->nav_bar_->addSearch(std::move(edit), Wt::AlignmentFlag::Right);

    // Create a popup widget for user account operations.
    impl_->user_account_popup_ = std::make_unique<Wt::WPopupWidget>(
        root()->removeWidget(impl_->auth_widget_));
    impl_->user_account_popup_->setTransient(true);

    impl_->auth_widget_->loggedInViewClicked().connect([=]() {
        impl_->user_account_popup_->hide();
    });

    auto item = right_menu->addItem(
        "", impl_->user_account_popup_.get(), &Wt::WWidget::show);
    // Set the profile icon image as background to the menu item text element.
    auto link = item->anchor();
    link->setStyleClass("nutpp-navbar-auth");
    link->widget(0)->setStyleClass(auth::AuthWidget::CSS_PROFILE_PIC_SM);
    // Bind the popup widget.
    impl_->user_account_popup_->setAnchorWidget(
        item, Wt::Orientation::Vertical);
    impl_->user_account_popup_->hidden().connect([=]() {
        right_menu->select(nullptr);
    });
}
} // namespace webserver
} // namespace nutpp
