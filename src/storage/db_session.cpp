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

#include "db_session.h"
#include "db_model.h"

namespace nutpp {
namespace storage {
// Constructor.
DbSession::DbSession(const DbModel &db, bool start_trans)
    : owned_session_(std::make_unique<Wt::Dbo::Session>()),
      session_(*owned_session_)
{
    db.initSession(session_);

    if (start_trans) {
        startTransaction();
    }
}

// Creates internal transaction.
void DbSession::startTransaction()
{
    if (!trans_) {
        trans_ = createTransaction();
    }
}

// Destroys internal transaction.
void DbSession::stopTransaction()
{
    if (trans_) {
        trans_.reset();
    }
}

// Creates external transaction.
std::unique_ptr<Wt::Dbo::Transaction> DbSession::createTransaction()
{
    return std::make_unique<Wt::Dbo::Transaction>(session_);
}

// Commits transaction.
bool DbSession::commit()
{
    if (trans_) {
        return trans_->commit();
    }

    // The internal transaction was not started, use a temporary one.
    auto t = createTransaction();
    return t->commit();
}

// Roll-backs active transaction.
void DbSession::rollback()
{
    if (trans_) {
        trans_->rollback();
    }
}

// Discards db session.
void DbSession::discard(const char *table_name)
{
    rollback();
    session_.rereadAll(table_name);
}
} // namespace storage
} // namespace nutpp
