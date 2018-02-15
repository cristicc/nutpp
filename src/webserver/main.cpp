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
#include "util/log_initializer.h"
#include "util/string_ops.h"

#include <signal.h>
#include <string.h>
#include <Wt/WServer.h>

// Wt logger setup.
#define LOGWT_LOGGER        "Nutpp/main: "
#define LOGWT_INFO(m)       Wt::log("info") << LOGWT_LOGGER << m
#define LOGWT_WARN(m)       Wt::log("warning") << LOGWT_LOGGER << m
#define LOGWT_ERROR(m)      Wt::log("error") << LOGWT_LOGGER << m
#define LOGWT_FATAL(m)      Wt::log("fatal") << LOGWT_LOGGER << m

LOGNUTPP_LOGGER_WS;

namespace nutpp {
namespace {
// Shared DB model instance.
storage::DbModel db_model;

// Catch signals.
void handleSignals(int signal)
{
    if (signal == SIGUSR1) {
        // Rotate Wt log.
        LOGWT_INFO("Rotating log file");
        // FIXME: This is a hack to add support for log file rotation in Wt
        // using a patched version of setFile().
        // Wt::WLogger &logger = Wt::WServer::instance()->logger();
        // logger.setFile("");
        LOGWT_INFO("Rotated log file");
    }
}

// Register signal handlers.
void registerSignals()
{
    // Block other terminal-generated signals while signal handler runs.
    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGHUP);
    sigaddset(&block_mask, SIGINT);
    sigaddset(&block_mask, SIGQUIT);
    sigaddset(&block_mask, SIGTERM);
    sigaddset(&block_mask, SIGTSTP);
    sigaddset(&block_mask, SIGUSR1);

    // Catch SIGUSR1 signal to trigger reopening of the Wt log.
    struct sigaction action;
    action.sa_handler = handleSignals;
    action.sa_mask = block_mask;
    action.sa_flags = SA_RESTART;

    if (sigaction(SIGUSR1, &action, nullptr) < 0) {
        LOGWT_WARN("Failed to register SIGUSR1 handler: " << strerror(errno));
    }
}

// Creates an application instance for a new session.
std::unique_ptr<Wt::WApplication> createApp(const Wt::WEnvironment &env)
{
    return std::make_unique<nutpp::webserver::NutppUI>(env, db_model);
}
} // namespace
} // namespace nutpp


// Configure and start the web server.
int main(int argc, char **argv)
{
    nutpp::util::LogInitializer log_init;
    Wt::WServer server(argv[0]);

    try {
        // Init web server.
        server.setServerConfiguration(argc, argv);

        // Get app root dir.
        std::string app_dir = Wt::WServer::instance()->appRoot();
        if (app_dir.back() == '/' || app_dir.back() == '\\') {
            app_dir.pop_back();
        }

        // Initialize application logger.
        if (!log_init.configure(
                nutpp::util::sanitizeFilePath(
                    nutpp::webserver::silentReadAppStringSetting(
                        "log4cplusConfig"),
                    app_dir),
                app_dir))
        {
            LOGWT_WARN("Failed to initialize logging");
        }

        // Set signal handlers.
        nutpp::registerSignals();

        // Initialize database.
        if (!nutpp::db_model.initialize(
                nutpp::util::sanitizeFilePath(
                    nutpp::webserver::readAppStringSetting("sqlite3Db"),
                    app_dir),
                nutpp::webserver::readAppIntSetting("maxDbConnections"))
            || !nutpp::db_model.createSchema())
        {
            LOGNUTPP_FATAL("Database corrupted or not accessible");
            return 1;
        }

        // Set web contexts.
        server.addEntryPoint(
            Wt::EntryPointType::Application, nutpp::createApp,
            nutpp::webserver::readAppStringSetting("deploymentURI"));

        if (server.start()) {
            LOGNUTPP_INFO("Web server started");

            int sig = Wt::WServer::waitForShutdown();
            LOGNUTPP_INFO("Web server stopped (signal = " << sig << ")");
            LOGWT_INFO("Web server stopped (signal = " << sig << ")");
            server.stop();
        }

        return 0;
    } catch (const Wt::WServer::Exception &e) {
        LOGWT_FATAL(e.what());
    } catch (const std::exception &e) {
        LOGWT_FATAL("exception: " << e.what());
    }

    return 1;
}
