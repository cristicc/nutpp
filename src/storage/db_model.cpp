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
bool DbModel::createSchema(bool force)
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

    int count = getUserCount();
    if (count > 0) {
        // Found at least one user, assume DB schema has been already created.
        return true;
    }

    if (count < 0) {
        // We got a failure when trying to count users, assume DB schema
        // has not been created yet.
        LOGNUTPP_INFO("Creating DB schema");

        try {
            s.createTables();
        } catch (const dbo::Exception &e) {
            LOGNUTPP_ERROR("Failed to create DB schema: " << e.what());
            return false;
        }
    }

    // We have the schema, but no users, let's create the default one.
    return createDefaultUser();
}

// Gets new session.
bool DbModel::initSession(dbo::Session &session) const
{
    try {
        // Enable the DB session to use the shared connection pool.
        session.setConnectionPool(*conn_pool_);

        // Map classes to database tables.
        session.mapClass<User>(User::kTableName);

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
bool DbModel::discardSession(dbo::Session &session, const char *tableName) const
{
    try {
        session.rereadAll(tableName);
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

// Creates default admin account.
bool DbModel::createDefaultUser()
{
    LOGNUTPP_INFO("Creating default user account");

    dbo::Session s;
    if (!initSession(s)) {
        return false;
    }

    try {
        auto user = std::make_unique<storage::User>();

        user->name = "Admin";
        user->email = "admin@local";
        user->passwd = "admin";
        user->role = storage::UserRole::ADMIN;

        s.add(std::move(user));
    } catch (const dbo::Exception &e) {
        LOGNUTPP_ERROR("Failed to create default user: " << e.what());
        return false;
    }

    return saveSession(s);
}
} // namespace storage
} // namespace nutpp
