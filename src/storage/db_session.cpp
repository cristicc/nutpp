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

namespace dbo = Wt::Dbo;

namespace nutpp {
namespace storage {
// Constructor.
DbSession::DbSession(const DbModel &db, bool create_trans)
{
    db.initSession(session_);

    if (create_trans) {
        createTransaction();
    }
}

// Creates transaction.
void DbSession::createTransaction()
{
    if (!trans_) {
        trans_ = std::make_unique<Wt::Dbo::Transaction>(session_);
    }
}

// Commits transaction.
bool DbSession::commit()
{
    createTransaction();
    return trans_->commit();
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
