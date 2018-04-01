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
 * @brief Wraper over @c Wt::Dbo::Session and @c Wt::Dbo::Transaction.
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
     * @param[in] start_trans Automatically starts the internally managed
     * transaction after initializing the session.
     *
     * @sa startTransaction()
     */
    DbSession(const DbModel &db, bool start_trans = true);

    /**
     * Instantiates the class using the session of a persisted database object.
     *
     * @param[in] db_obj The persisted database object.
     * @param[in] start_trans Automatically starts the internally managed
     * transaction.
     */
    template<class C> DbSession(const Wt::Dbo::ptr<C> &db_obj,
                                bool start_trans = true);

    /**
     * @brief Starts the internally managed transaction.
     *
     * Does nothing if the internal transaction has been already started.
     */
    void startTransaction();

    /**
     * @brief Stops the internally managed transaction.
     *
     * Any changes made during the transaction will be committed automatically.
     * Does nothing if the internal transaction has not been already started.
     */
    void stopTransaction();

    /**
     * @brief Creates a user transaction not managed internally.
     * @return A transaction instance owned by the caller.
     */
    std::unique_ptr<Wt::Dbo::Transaction> createTransaction();

    /**
     * @brief Wrapper for @c Wt::Dbo::Session::add(std::unique_ptr<C>).
     */
    template<class C> Wt::Dbo::ptr<C> add(std::unique_ptr<C> obj);

    /**
     * @brief Wrapper for @c Wt::Dbo::Session::add(Wt::Dbo::ptr<C>).
     */
    template<class C> Wt::Dbo::ptr<C> add(Wt::Dbo::ptr<C> &obj);

    /**
     * @brief Commits the internally managed transaction.
     *
     * Automatically starts a temporary transaction if the internal managed
     * transaction has not been started in order to flush any changes done
     * on the database during the current session.
     *
     * @return @c true if the transaction was flushed to the database.
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
    // Session object owned by this class.
    std::unique_ptr<Wt::Dbo::Session> owned_session_;
    // Transaction object owned by this class.
    std::unique_ptr<Wt::Dbo::Transaction> trans_;
    // Reference to owned_session_ or to a session obtained from a DB object.
    Wt::Dbo::Session &session_;
};

// Constructor.
template<class C>
DbSession::DbSession(const Wt::Dbo::ptr<C> &db_obj, bool start_trans)
    : session_(*db_obj.session())
{
    if (start_trans) {
        startTransaction();
    }
}

// Wrapper.
template<class C>
Wt::Dbo::ptr<C> DbSession::add(std::unique_ptr<C> obj)
{
    return session_.add(std::move(obj));
}

// Wrapper.
template<class C>
Wt::Dbo::ptr<C> DbSession::add(Wt::Dbo::ptr<C> &obj)
{
    return session_.add(obj);
}
} // namespace storage
} // namespace nutpp
#endif // NUTPP_STORAGE_DBSESSION_H_
