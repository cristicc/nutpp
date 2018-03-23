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
 * @brief Database session.
 */

#ifndef NUTPP_STORAGE_DBSESSION_H_
#define NUTPP_STORAGE_DBSESSION_H_

#include <Wt/Dbo/Session.h>

namespace nutpp {
namespace storage {
class DbModel;

/**
 * @brief Wraps a @c Wt::Dbo::Session instance.
 *
 * The class is able to automatically create a transaction
 * associated to the database session.
 */
class DbSession {
public:
    /// Copy constructor not allowed.
    DbSession(const DbSession &) = delete;

    /// Move constructor not allowed.
    DbSession(DbSession &&) = delete;

    /// Assignment not allowed.
    DbSession &operator=(const DbSession &) = delete;

    /// Move assignment not allowed.
    DbSession &operator=(DbSession &&) = delete;

    /**
     * @brief Creates a database session using a connection
     * from the given model.
     *
     * @param[in] db The database model containing the connection pool.
     * @param[in] create_trans Automatically create a transaction after
     * initializing the session.
     */
    DbSession(const DbModel &db, bool create_trans = true);

    /**
     * @brief Creates a transaction.
     *
     * Does nothing if a transaction has been already created.
     */
    void createTransaction();

    /**
     * @brief Commits the active transaction.
     *
     * Automatically creates a new transaction if none has been initialized
     * in order to flush any changes done on the database session.
     *
     * @return @c true if the transaction was flushed to the database.
     * @sa createTransaction()
     */
    bool commit();

    /**
     * @brief Rolls back the active transaction.
     *
     * Does nothing if no transaction has been created.
     */
    void rollback();

    /**
     * @brief Discards the given session.
     *
     * Rereads all objects from the database, possibly discarding unflushed
     * modifications.
     *
     * @param[in] table_name If specified, only the objects
     * of that table are reread.
     */
    void discard(const char *table_name = nullptr);

    /// Returns a reference to the underlying Dbo session.
    Wt::Dbo::Session &getDboSession() { return session_; }

private:
    Wt::Dbo::Session session_;
    std::unique_ptr<Wt::Dbo::Transaction> trans_;
};
} // namespace storage
} // namespace nutpp
#endif // NUTPP_STORAGE_DBSESSION_H_
