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
// FIXME: some methods could be static

/**
 * @brief Implements a model for database persistence.
 */
class DbModel {
public:
    /**
     * @brief Creates an uninitialized DB model instance.
     * @sa initialize()
     */
    DbModel();

    /**
     * @brief Creates an initialized the DB model instance.
     * @param[in] db_file_path Path to a sqlite3 file.
     * @param[in] max_conn Max no. of connections in the pool.
     */
    DbModel(const std::string &db_file_path, int max_conn);

    /// Destructor.
    ~DbModel();

    /**
     * @brief Initialized the current DB model instance.
     * @param[in] db_file_path Path to a sqlite3 file.
     * @param[in] max_conn Max no. of connections in the pool.
     */
    bool initialize(const std::string &db_file_path, int max_conn);

    /**
     * @brief Creates the database schema.
     *
     * The method verifies if the user table exists and attempts to create
     * all the necessary objects, including a default user account with
     * administrative role.
     *
     * @param[in] force Enables a database reset by deleting all tables
     * followed by their re-creation.
     * @return @c false if errors occurred, or @c true otherwise.
     */
    bool createSchema(bool force = false);

    /**
     * @brief Initializes a database session using a connection from the pool.
     * @param[in,out] session The uninitialized database session.
     * @param[in] auth_only Only maps the classes relevant for authentication.
     * @return @c false if errors occurred, or @c true otherwise.
     */
    bool initSession(Wt::Dbo::Session &session, bool auth_only = false) const;

    /**
     * @brief Saves the given database session.
     * @param[in,out] session The database session.
     * @return @c false if errors occurred, or @c true otherwise.
     */
    bool saveSession(Wt::Dbo::Session &session) const;

    /**
     * @brief Discards the given session.
     *
     * Rereads all objects from the database, possibly discarding unflushed
     * modifications.
     *
     * @param[in,out] session The database session.
     * @param[in] tableName If specified, only the objects of that table are
     * reread.
     * @return @c false if errors occurred, or @c true otherwise.
     */
    bool discardSession(Wt::Dbo::Session &session,
                        const char *table_name = nullptr) const;

private:
    // Returns the no. of user accounts or -1 in case of errors.
    int getUserCount() const;

    // Creates default admin account.
    bool createDefaultUser();

    // Provides a connection pool for the application DB.
    std::unique_ptr<Wt::Dbo::SqlConnectionPool> conn_pool_;
};
} // namespace storage
} // namespace nutpp
#endif // NUTPP_STORAGE_DB_MODEL_H_
