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

#include "nutpp-ui.h"

#include "util/log.h"
#include "util/settings.h"

#include <signal.h>
#include <string.h>

#include <Wt/WServer.h>

/*
 * Wt logger setup.
 */
#define LOGWT_LOGGER            "Webcfg/main: "
#define LOGWT_INFO(m)           Wt::log("info") << LOGWT_LOGGER << m
#define LOGWT_WARN(m)           Wt::log("warning") << LOGWT_LOGGER << m
#define LOGWT_ERROR(m)          Wt::log("error") << LOGWT_LOGGER << m
#define LOGWT_FATAL(m)          Wt::log("fatal") << LOGWT_LOGGER << m

/*
 * Create a Connect2UI application for a new web session.
 */
namespace {
/*
 * Instantiates the application.
 */
std::unique_ptr<Wt::WApplication> createApp(const Wt::WEnvironment &env)
{
    LOGWT_INFO("Creating nutpp application");
    return std::make_unique<nutpp::webserver::NutppUI>(env);
}

/*
 * Signal handler to trigger log rotation.
 */
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

/*
 * Catch SIGTSTP, SIGUSR1 & SIGUSR2 signals to trigger reopening
 * of the core and the application logs.
 */
void registerSignals()
{
    /* Block other terminal-generated signals while handler runs. */
    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGHUP);
    sigaddset(&block_mask, SIGINT);
    sigaddset(&block_mask, SIGQUIT);
    sigaddset(&block_mask, SIGTERM);
    sigaddset(&block_mask, SIGTSTP);
    sigaddset(&block_mask, SIGUSR1);
    sigaddset(&block_mask, SIGUSR2);

    struct sigaction action;
    action.sa_handler = handleSignals;
    action.sa_mask = block_mask;
    action.sa_flags = SA_RESTART;

    if (sigaction(SIGUSR1, &action, nullptr) < 0) {
        LOGWT_WARN("Failed to register SIGUSR1 handler: " << strerror(errno));
    }
}

/*
 * One-time initialization.
 */
void initWebApp()
{
    LOGWT_INFO("one-time initialization");

    std::string app_dir = Wt::WServer::instance()->appRoot();
    if (app_dir.back() == '/' || app_dir.back() == '\\') {
        app_dir.pop_back();
    }

    std::string log_file;
    if (nutpp::util::Log::getInstance().initialize(
            app_dir,
            nutpp::util::silentReadAppStringSetting("loggerConfigName"),
            log_file))
    {
        LOGWT_INFO("Store application logs in: " << log_file);
    } else {
        LOGWT_WARN("Failed to init logger component");
    }

    /* Handle signals for log rotation. */
    registerSignals();
}

/*
 * Release any static resources.
 */
void cleanupWebApp()
{
    nutpp::util::Log::getInstance().shutdown();
}
} // namespace

/*
 * Configure and run the web server.
 */
int main(int argc, char **argv)
{
    try {
        Wt::WServer server(argv[0]);

        /* Configure the HTTP server. */
        server.setServerConfiguration(argc, argv);

        /* Webcfg specific initialization. */
        initWebApp();

        /* Set application entry points. */
        server.addEntryPoint(
            Wt::EntryPointType::Application, createApp,
            nutpp::util::readAppStringSetting("deploymentURI"));

        if (server.start()) {
            int sig = Wt::WServer::waitForShutdown();
            LOGNUTPP_INFO("Shutdown (signal = " << sig << ")");
            LOGWT_INFO("shutdown (signal = " << sig << ")");
            server.stop();
        }

        /* Webcfg specific cleanup. */
        cleanupWebApp();
        return 0;
    } catch (Wt::WServer::Exception &e) {
        LOGWT_FATAL(e.what());
    } catch (std::exception &e) {
        LOGWT_FATAL("exception: " << e.what());
    }

    return 1;
}
