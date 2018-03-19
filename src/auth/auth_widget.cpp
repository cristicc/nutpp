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

#include "auth_widget.h"

#include "login_session.h"
#include "registration_view.h"
#include "util/log.h"
#include "util/string_ops.h"

#include <Wt/Auth/OAuthWidget.h>
#include <Wt/Http/Client.h>
#include <Wt/Utils.h>
#include <Wt/WApplication.h>
#include <Wt/WLink.h>
#include <Wt/WPushButton.h>

LOGNUTPP_LOGGER_AUTH;

namespace nutpp {
namespace auth {
// CSS
const char *AuthWidget::CSS_PROFILE_PIC_SM = "nutpp-auth-profile-pic-sm";
const char *AuthWidget::CSS_PROFILE_PIC_LG = "nutpp-auth-profile-pic-lg";

// C-tor.
AuthWidget::AuthWidget(LoginSession &session)
    : Wt::Auth::AuthWidget(
        LoginSession::auth(), session.users(), session.login()),
    session_(session),
    profile_picture_sm_(
        std::make_unique<Wt::WLink>("/images/default_user_profile_pic_64.png")),
    profile_picture_lg_(
        std::make_unique<Wt::WLink>("/images/default_user_profile_pic_64.png"))
{}

// Destructor in .cpp required by unique_ptr to avoid incomplete type errors.
AuthWidget::~AuthWidget() = default;

// Creates registration view.
std::unique_ptr<Wt::WWidget> AuthWidget::createRegistrationView(
    const Wt::Auth::Identity &id)
{
    auto model = createRegistrationModel();
    if (login().loggedIn()) {
        model->setValue(Wt::Auth::RegistrationModel::LoginNameField,
            login().user().identity(Wt::Auth::Identity::LoginName));
    }

    if (id.isValid()) {
        model->registerIdentified(id);
    }

    //FIXME: get rid of session_ attribute and pass only the necessary info
    auto view = std::make_unique<RegistrationView>(session_, this);
    view->setModel(std::move(model));
    return std::move(view);
}

// Creates an OAuth login widget.
void AuthWidget::createOAuthLoginView()
{
    if (!model()->oAuth().empty()) {
        setCondition("if:oauth", true);

        Wt::WContainerWidget *icons
            = bindWidget("icons", std::make_unique<Wt::WContainerWidget>());
        icons->setInline(isInline());

        for (unsigned i = 0; i < model()->oAuth().size(); ++i) {
            const Wt::Auth::OAuthService *service = model()->oAuth()[i];

            Wt::Auth::OAuthWidget *w = icons->addWidget(
                std::make_unique<Wt::Auth::OAuthWidget>(*service));
            w->setImageLink("images/oauth-" + service->name() + ".png");

            w->authenticated().connect([=](
                Wt::Auth::OAuthProcess *oauth,
                const Wt::Auth::Identity& identity)
            {
                if (identity.isValid()) {
                    LOGNUTPP_INFO(
                        "OAuth " << oauth->service().name()
                        << " identity confirmed: " << identity.id()
                        << ", " << identity.name() << ", " << identity.email());

                    fetchProfilePicture(oauth);

                    std::unique_ptr<Wt::Auth::AbstractUserDatabase::Transaction>
                        t(model()->users().startTransaction());

                    Wt::Auth::User user
                        = model()->baseAuth()->identifyUser(
                            identity, model()->users());

                    if (user.isValid()) {
                        model()->loginUser(login(), user);
                    } else {
                        registerNewUser(identity);
                    }

                    if (t.get()) {
                        //FIXME: handle db errors
                        t->commit();
                    }
                } else {
                    LOGNUTPP_ERROR(
                        "OAuth " << oauth->service().name()
                        << " identity not confirmed: " << oauth->error());
                    displayError(oauth->error());
                }
            });
        }
    }
}

// Logged-in view.
void AuthWidget::createLoggedInView()
{
    setTemplateText(tr("nutpp.auth.template.logged-in"));

    wApp->styleSheet().addRule(
        "." + std::string(CSS_PROFILE_PIC_SM),
        "background-image: url('" + profile_picture_sm_->url() + "')");
    wApp->styleSheet().addRule(
        "." + std::string(CSS_PROFILE_PIC_LG),
        "background-image: url('" + profile_picture_lg_->url() + "')");

    auto profile_templ = std::make_unique<Wt::WTemplate>(
        "<a aria-label=\"Change profile picture\" href=\"\">"
        "<div class=\"" + std::string(CSS_PROFILE_PIC_LG)
        + "\" title=\"" + Wt::WString::tr("nutpp.auth.profile-pic")
        + "\"></div><span>" + Wt::WString::tr("nutpp.auth.profile-pic-change")
        + "</span></a>");

    bindWidget("user-profile-pic", std::move(profile_templ));
    bindString(
        "user-name",
        login().user().identity(Wt::Auth::Identity::LoginName));
    bindString("user-email", login().user().email());

    bindWidget(
        "edit-account-button",
        std::make_unique<Wt::WPushButton>(
            Wt::WString::tr("nutpp.auth.edit-account")))
                ->clicked().connect([=]() {
                    logged_in_clicked_.emit();
                });

    bindWidget(
        "add-account-button",
        std::make_unique<Wt::WPushButton>(
            Wt::WString::tr("nutpp.auth.add-account")))
                ->clicked().connect([=]() {
                    logged_in_clicked_.emit();
                    registerNewUser();
                });

    bindWidget(
        "logout-button",
        std::make_unique<Wt::WPushButton>(
            Wt::WString::tr("nutpp.auth.sign-out")))
                ->clicked().connect([=]() {
                    logged_in_clicked_.emit();
                    model()->logout(login());
                });

    //FIXME: not working
    escapePressed().connect([=]() {
        logged_in_clicked_.emit();
    });
}

// JSON parsing utilities based on Wt/Auth/OidcService.C.
namespace {
    std::string parseClaims(const Wt::Json::Object& claims)
    {
        //TODO: fetch language
        //claims.get("locale").orIfNull("")
        return claims.get("picture").orIfNull("");
    }

    bool parseIdToken(const std::string& id_token, std::string &result) {
        std::vector <std::string> parts;
        util::split(id_token, ".", parts);

        if (parts.size() != 3) {
            LOGNUTPP_ERROR("Malformed id_token: '" << id_token << "'");
            return false;
        }

        Wt::Json::Object payload_json;
        Wt::Json::ParseError err;

        bool ok = Wt::Json::parse(
            Wt::Utils::base64Decode(parts[1]), payload_json, err, false);
        if (!ok) {
            LOGNUTPP_ERROR("Failed to parse id_token: '" << err.what());
            return false;
        }

        result = parseClaims(payload_json);
        return !result.empty();
    }
} // namespace

// Fetches the url of the profile image.
void AuthWidget::fetchProfilePicture(Wt::Auth::OAuthProcess *oauth) {
    auto &token = oauth->token();

    if (!token.idToken().empty()) {
        std::string result;
        if (parseIdToken(token.idToken(), result)) {
            setProfilePicture(result);
            return;
        }
    }

    //FIXME: done() signal never called
    http_client_ = std::make_unique<Wt::Http::Client>();
    http_client_->setTimeout(std::chrono::seconds(10));
    http_client_->setMaximumResponseSize(10 * 1024);

    http_client_->done().connect(
        [=](Wt::AsioWrapper::error_code err, const Wt::Http::Message& response)
        {
            Wt::WApplication::instance()->resumeRendering();

            if (err) {
                LOGNUTPP_WARN("User info request failed: " << err.message());
                //authenticated().emit(Identity::Invalid);
                return;
            }

            LOGNUTPP_TRACE("User info request body: " << response.body());

            if (response.status() != 200) {
                LOGNUTPP_WARN("User info request returned: " << response.status());
                return;
            }

            Wt::Json::Object user_info;
            Wt::Json::ParseError e;
            bool ok = Wt::Json::parse(response.body(), user_info, e);

            if (ok) {
                //authenticated().emit(parseClaims(userInfo));
                std::string result = parseClaims(user_info);
                if (!result.empty()) {
                    setProfilePicture(result);
                }
            } else {
                LOGNUTPP_WARN("Failed to parse user info: '"
                               << response.body() << "'");
            }
        }
    );

    std::vector<Wt::Http::Message::Header> headers;
    headers.push_back(
        Wt::Http::Message::Header(
            "Authorization", "Bearer " + token.value()));

    LOGNUTPP_DEBUG("Fetching user info from: "
                   << oauth->service().userInfoEndpoint());
    if (http_client_->get(oauth->service().userInfoEndpoint(), headers)) {
        // Wait for resume.
        Wt::WApplication::instance()->deferRendering();
    } else {
        LOGNUTPP_WARN("Failed to request user info from: "
                      << oauth->service().userInfoEndpoint());
    }
}

// Stores the received url.
void AuthWidget::setProfilePicture(const std::string &url)
{
    //TODO: set url based on image size
    profile_picture_sm_->setUrl(url);
    profile_picture_lg_->setUrl(url);

    LOGNUTPP_DEBUG("Set profile picture URL: " << url);
}
} // namespace auth
} // namespace nutpp
