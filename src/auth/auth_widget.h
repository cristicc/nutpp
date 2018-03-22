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
 * @brief Custom authentication widget.
 */

#ifndef NUTPP_AUTH_AUTHWIDGET_H_
#define NUTPP_AUTH_AUTHWIDGET_H_

#include <Wt/Auth/AuthWidget.h>

namespace nutpp {
namespace auth {
// Fwd declaration.
class LoginSession;

/**
 * @brief A custom authentication widget.
 *
 * Extends the Wt's @c AuthWidget to provide a custom registration view.
 * It uses a LoginSession instance for managing the user authentication.
 *
 * @sa RegistrationView
 * @sa UserDetailsModel
 */
class AuthWidget : public Wt::Auth::AuthWidget {
public:
    /**
     * Name of the CSS class for the small (32x32) profile picture.
     */
    static const char *CSS_PROFILE_PIC_SM;

    /**
     * Name of the CSS class for the large (64x64 or 96x96) profile picture.
     */
    static const char *CSS_PROFILE_PIC_LG;

    /**
     * @brief Creates a class instance.
     * @param[in] session A reference to a LoginSession instance.
     */
    AuthWidget(LoginSession &session);

    /// Destructor.
    ~AuthWidget();

    /**
     * @brief Signal that indicates the logged in view should
     * be hidden.
     *
     * This signal is emitted as a result of a click on one of the
     * LoggedIn view controls.
     */
    Wt::Signal<>& loggedInViewClicked() { return logged_in_clicked_; }

protected:
    /**
     * @brief Creates a custom registration widget using
     * RegistrationView and UserDetailsModel.
     */
    virtual std::unique_ptr<Wt::WWidget> createRegistrationView(
        const Wt::Auth::Identity &id) override;

    /**
     * @brief Creates a widget to login using OAuth.
     */
    virtual void createOAuthLoginView() override;

    /**
     * @brief Creates a custom view shown when the user is logged in.
     *
     * The implementation renders the
     * <tt>"nutpp.auth.template.logged-in"</tt> template.
     */
    virtual void createLoggedInView();

private:
    void fetchProfilePicture(Wt::Auth::OAuthProcess *oauth);
    void setProfilePicture(const std::string &url);

    Wt::Signal<> logged_in_clicked_;

    LoginSession &session_;

    std::unique_ptr<Wt::WLink> profile_picture_sm_;
    std::unique_ptr<Wt::WLink> profile_picture_lg_;
    std::unique_ptr<Wt::Http::Client> http_client_;
};
} // namespace storage
} // namespace nutpp
#endif // NUTPP_AUTH_AUTHWIDGET_H_
