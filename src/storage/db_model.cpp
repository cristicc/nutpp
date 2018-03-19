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

#include "db_model.h"

#include "user.h"
#include "util/log.h"

#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/Dbo/Exception.h>
#include <Wt/Dbo/FixedSqlConnectionPool.h>

LOGNUTPP_LOGGER_STORAGE;

namespace dbo = Wt::Dbo;

namespace nutpp {
namespace storage {
// Constructor.
DbModel::DbModel()
{}

// Constructor.
DbModel::DbModel(const std::string &db_file_path, int max_conn)
{
    if (!initialize(db_file_path, max_conn)) {
        throw std::runtime_error("Database not accessible");
    }
}

// Destructor in .cpp required by unique_ptr to avoid incomplete type errors.
DbModel::~DbModel() = default;


// Initialize model.
bool DbModel::initialize(const std::string &db_file_path, int max_conn)
{
    LOGNUTPP_INFO("Opening DB '" << db_file_path << "'");

    try {
        // Opens a database connection.
        auto sqlite3_conn
            = std::make_unique<dbo::backend::Sqlite3>(db_file_path);

        // Enable SQL debugging in the Wt log.
        if (LOGNUTPP_TRACE_ENABLED) {
            sqlite3_conn->setProperty("show-queries", "true");
        }

        // Create the DB connection pool.
        conn_pool_ = std::make_unique<dbo::FixedSqlConnectionPool>(
            std::move(sqlite3_conn), max_conn);

        return true;
    } catch (const dbo::Exception &e) {
        LOGNUTPP_ERROR("Failed to initialize DB '"
                       << db_file_path << "': " << e.what());
    }

    return false;
}

// Creates schema and default user.
bool DbModel::createSchema(int &users, bool force)
{
    dbo::Session s;
    if (!initSession(s)) {
        return false;
    }

    if (force) {
        try {
            s.dropTables();
        } catch (const dbo::Exception &e) {}
    }

    users = getUserCount();
    if (users > 0) {
        // Found at least one user, assume DB schema has been already created.
        return true;
    }

    if (users < 0) {
        // We got a failure when trying to count users, assume DB schema
        // has not been created yet.
        LOGNUTPP_INFO("Creating DB schema");

        try {
            s.createTables();
            users = 0;
        } catch (const dbo::Exception &e) {
            LOGNUTPP_ERROR("Failed to create DB schema: " << e.what());
            return false;
        }
    }

    return true;
}

// Gets new session.
bool DbModel::initSession(dbo::Session &session, bool auth_only) const
{
    try {
        // Enable the DB session to use the shared connection pool.
        session.setConnectionPool(*conn_pool_);

        // Map authentication related classes to DB tables.
        session.mapClass<User>(User::kTableName);
        session.mapClass<Patient>(Patient::kTableName);
        session.mapClass<AuthInfo>("auth_info");
        session.mapClass<AuthInfo::AuthIdentityType>("auth_identity");
        session.mapClass<AuthInfo::AuthTokenType>("auth_token");

        if (!auth_only) {
            // Map app specific classes to DB tables.
            //TODO: check if this is even possible, otherwise get rid of
            // auth_only parameter.
        }

        return true;
    } catch (const dbo::Exception &e) {
        LOGNUTPP_ERROR("Failed to create new DB session: " << e.what());
    }

    return false;
}

// Saves session.
bool DbModel::saveSession(dbo::Session &session) const
{
    try {
        dbo::Transaction t(session);

        return t.commit();
    } catch (const dbo::Exception &e) {
        LOGNUTPP_ERROR("Failed to save DB session: " << e.what());
    }

    return false;
}

// Discards session.
bool DbModel::discardSession(
    dbo::Session &session, const char *table_name) const
{
    try {
        session.rereadAll(table_name);
    } catch (const dbo::Exception &e) {
        LOGNUTPP_ERROR("Failed to discard DB session: " << e.what());
        return false;
    }

    return true;
}

// Counts users.
int DbModel::getUserCount() const
{
    dbo::Session s;
    if (!initSession(s)) {
        return false;
    }

    dbo::Transaction t(s);
    try {
        return s.query<int>(
            std::string("select count(1) from ") + storage::User::kTableName);
    } catch (const dbo::Exception &e) {}

    return -1;
}
} // namespace storage
} // namespace nutpp
