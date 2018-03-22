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
 * @brief Web application UI.
 */

#ifndef NUTPP_WEBSERVER_NUTPPUI_H_
#define NUTPP_WEBSERVER_NUTPPUI_H_

#include <Wt/WApplication.h>

/**
 * @brief Macro to access the nutpp::webserver::NutppUI instance
 * for the current session.
 *
 * @warning To be used only after the NutppUI instance has been created,
 * so NOT during the execution of the NutppUI constructor.
 */
#define NUTPP_APP   static_cast<nutpp::webserver::NutppUI *>(wApp)

/**
 * @brief Macro to access the nutpp::webserver::NutppUI instance
 * for the current session.
 *
 * @warning To be used only after the NutppUI instance has been created,
 * so NOT during the execution of the NutppUI constructor.
 */
#define NUTPP_DB    NUTPP_APP->getDbModel()

/**
 * @brief Namespace for the @e Nutpp application.
 */
namespace nutpp {
namespace storage {
// Fwd declaration.
class DbModel;
} // namespace storage

/**
 * @brief Namespace containing the sources for the webserver component.
 */
namespace webserver {
/**
 * @brief Represents an application instance for a single Web session.
 *
 * The class implements a Web user interface for the @e Nutpp application.
 * Each user session of the application has a corresponding NutppUI instance.
 */
class NutppUI : public Wt::WApplication {
public:
    /**
     * @brief Creates an application instance for the current session.
     *
     * @param[in] env Wt environment.
     * @param[in] db Database model.
     */
    NutppUI(const Wt::WEnvironment &env, const storage::DbModel &db);
    ~NutppUI();

    /**
     * @brief Gets access to the DB model instance.
     * @return The DB model instance shared between all web sessions.
     */
    const storage::DbModel &getDbModel();

private:
    // Specialization to handle application refresh
    void refresh() override;

    // Handler for logged in user.
    void handleLoggedIn(bool automatic);

    // Create components.
    void createNavBar();

    // Hide implementation details.
    class NutppUIImpl;
    std::unique_ptr<NutppUIImpl> impl_;
};
} // namespace webserver
} // namespace nutpp
#endif // NUTPP_WEBSERVER_NUTPPUI_H_
