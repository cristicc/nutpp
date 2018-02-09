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

#include "util/log.h"

#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/Dbo/FixedSqlConnectionPool.h>

namespace nutpp {
namespace storage {
// Constructor.
DbModel::DbModel(const std::string &db_file_path, int max_conn)
    : db_file_path_(db_file_path),
    max_conn_(max_conn)
{}

// Destructor in .cpp required by unique_ptr to avoid incomplete type errors.
DbModel::~DbModel() = default;

// Gets new session.
void DbModel::newSession(Wt::Dbo::Session &session)
{
    if (!conn_pool_) {
        // Create an sqlite3 connection.
        auto sqlite3_conn
            = std::make_unique<Wt::Dbo::backend::Sqlite3>(db_file_path_);

        // Enable SQL debugging in the Wt application log
        if (LOGNUTPP_DEBUG_ENABLED) {
            sqlite3_conn->setProperty("show-queries", "true");
        }

        // Create the DB connection pool.
        conn_pool_ = std::make_unique<Wt::Dbo::FixedSqlConnectionPool>(
            std::move(sqlite3_conn), max_conn_);
    }

    // Enable the DB session to use the shared connection pool.
    session.setConnectionPool(*conn_pool_);
}
} // namespace storage
} // namespace nutpp
