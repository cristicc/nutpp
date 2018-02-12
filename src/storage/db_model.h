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
 * @brief Database model.
 */

#ifndef NUTPP_STORAGE_DB_MODEL_H_
#define NUTPP_STORAGE_DB_MODEL_H_

#include <string>
#include <Wt/Dbo/Session.h>

namespace nutpp {
/**
 * @brief Namespace containing the sources for the data persistence component.
 */
namespace storage {
/**
 * @brief Implements a model for database persistence.
 */
class DbModel {
public:
    /**
     * @brief Initializes the DB model.
     * @param[in] db_file_path Path to a sqlite3 file.
     * @param[in] max_conn Max no. of connections in the pool.
     */
    DbModel(const std::string &db_file_path, int max_conn);

    // Destructor.
    ~DbModel();

    /**
     * @brief Creates a database session using a connection from the pool.
     * @param[out] session The new database session.
     */
    void newSession(Wt::Dbo::Session &session);

private:
    std::string db_file_path_;
    int max_conn_;
    // Provides a connection pool for the application DB.
    std::unique_ptr<Wt::Dbo::SqlConnectionPool> conn_pool_;
};
} // namespace storage
} // namespace nutpp
#endif /* NUTPP_STORAGE_DB_MODEL_H_ */
