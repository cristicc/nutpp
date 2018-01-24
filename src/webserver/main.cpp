/*
 * main.cpp
 */

#include "Connect2UI.h"
#include "HomeServersConfig.h"
#include "SystemController.h"
#include "dbus.h"

#include <Wt/WServer.h>

/*
 * Deployment paths
 */
#define DEPLOY_PATH_WEBFULL     "/UserInterfaces"
#define DEPLOY_PATH_WEBLITE     "/BasicInterfaces"

/*
 * Environment variable to be used for enabling application local deployment.
 * That means the application will NOT use the system bus for the communication
 * with the core services, but the session bus.
 * Also a local path will be used for the application configuration data,
 * that is relative to the Wt application root.
 */
#define ENV_RSG_LOCAL_DEPLOY    "RSG_LOCAL_DEPLOYMENT"

/*
 * Wt logger setup
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
std::unique_ptr<Wt::WApplication> createApp(
    const Wt::WEnvironment &env, Connect2UIType appType)
{
    LOGCNT_DEBUG(
        "Creating " << (appType == Connect2UIType::Full ? "full" : "lite")
        << " application");

    return std::make_unique<Connect2UI>(env, appType);
}
} // namespace

/*
 * Signal handler to trigger log rotation.
 */
static void handleSignals(int signal)
{
    if (signal == SIGTSTP) {
        // FIXME: This is a hack to add support for log file rotation in Wt
        // Rotate Wt log
        LOGWT_INFO("Rotating log file");
        Wt::WLogger &logger = Wt::WServer::instance()->logger();
        // This is a patched version of setFile()
        logger.setFile("");
        LOGWT_INFO("Rotated log file");
    } else if (signal == SIGUSR1) {
        // Rotate log4cplus event log
        RSGLogger::getInstance().reopenEventLog();
    } else if (signal == SIGUSR2) {
        // Rotate log4cplus audit log
        RSGLogger::getInstance().reopenAuditLog();
    }
}

/*
 * Catch SIGTSTP, SIGUSR1 & SIGUSR2 signals to trigger reopening
 * of the core and the application logs.
 */
static void registerSignals()
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

    /* Handle SIGTSTP signal for the core log rotation */
    if (sigaction(SIGTSTP, &action, nullptr) < 0) {
        LOGWT_WARN("Failed to register SIGTSTP handler: " << strerror(errno));
    }

    /* Handle SIGUSR signals for the app logs rotation */
    if (sigaction(SIGUSR1, &action, nullptr) < 0) {
        LOGWT_WARN("Failed to register SIGUSR1 handler: " << strerror(errno));
    }

    if (sigaction(SIGUSR2, &action, nullptr) < 0) {
        LOGWT_WARN("Failed to register SIGUSR2 handler: " << strerror(errno));
    }
}

/*
 * One-time initialization.
 */
static void initWebcfgApp()
{
    LOGWT_INFO("one-time initialization");

    char *env_var = getenv(ENV_RSG_LOCAL_DEPLOY);
    bool localDeployment = (env_var && (!strcasecmp(env_var, "1")
                                        || !strcasecmp(env_var, "YES")
                                        || !strcasecmp(env_var, "TRUE")));

    /* Initialize RSGLogger */
    if (RSGLogger::getInstance().init(
            Wt::WServer::instance()->appRoot(), !localDeployment))
    {
        LOGWT_INFO("Store application logs in: "
                   << RSGLogger::getInstance().getLogDir());
    } else {
        LOGWT_WARN("Failed to init RSGLogger");
    }

    /* Initialize RSGClientProxy */
    if (!RSGClientProxy::getInstance().init(!localDeployment)) {
        LOGCNT_ERROR("Failed to init RSGClientProxy");
    }

    /* Initialize SystemController */
    if (!SystemController::getInstance().init(localDeployment)) {
        LOGCNT_ERROR("Failed to init SystemController");
    }

    /* Initialize HomeServersConfig */
    if (!HomeServersConfig::getInstance()
        .load(SystemController::getInstance().getWebconfigDir()))
    {
        LOGCNT_ERROR("Failed to load HomeServersConfig");
    }

    /* Handle signals for log rotation */
    registerSignals();
}

/*
 * Release any static resources.
 */
static void cleanupWebcfgApp()
{
    RSGClientProxy::getInstance().cleanup();
    RSGLogger::getInstance().cleanup();
}

/*
 * Configure and run the web server.
 */
int main(int argc, char **argv)
{
    try {
        Wt::WServer server(argv[0]);

        /* This will block in case the process is the FastCGI relay server */
        server.setServerConfiguration(argc, argv);

        /* Webcfg specific initialization */
        initWebcfgApp();

        /* Set application entry points */
        server.addEntryPoint(
            Wt::EntryPointType::Application,
            std::bind(createApp, std::placeholders::_1, Connect2UIType::Full),
            DEPLOY_PATH_WEBFULL);
        server.addEntryPoint(
            Wt::EntryPointType::Application,
            std::bind(createApp, std::placeholders::_1, Connect2UIType::Lite),
            DEPLOY_PATH_WEBLITE);

        if (server.start()) {
            int sig = Wt::WServer::waitForShutdown();
            LOGCNT_INFO("Shutdown (signal = " << sig << ")");
            LOGWT_INFO("shutdown (signal = " << sig << ")");
            server.stop();
        }

        /* Webcfg specific cleanup */
        cleanupWebcfgApp();
        return 0;

    } catch (Wt::WServer::Exception &e) {
        LOGWT_FATAL(e.what());
    } catch (std::exception &e) {
        LOGWT_FATAL("exception: " << e.what());
    }

    return 1;
}
