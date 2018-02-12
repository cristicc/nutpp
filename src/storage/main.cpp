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
#include "util/log_initializer.h"

#include <iostream>

LOGNUTPP_LOGGER_STORAGE;

namespace db = nutpp::storage;

/*
 * Utility to perform database operations.
 */
int main(int argc, char **argv)
{
    // Initialize logging.
    nutpp::util::LogInitializer log_init;
    std::string log_file;
    if (log_init.configure(".", "log4cplus.properties", log_file)) {
        std::cerr << "Logging set to: " << log_file << std::endl;
    } else {
        std::cerr << "Failed to initialize logging" << std::endl;
    }

    // TODO: design db classes to remove Wt::Dbo objects from the API
    LOGNUTPP_INFO("Initializing db connection");
    db::DbModel model("test.db", 1);

    Wt::Dbo::Session session;
    model.newSession(session);
    try {
        session.mapClass<db::User>("user");
    } catch (Wt::Dbo::Exception &e) {
        LOGNUTPP_ERROR("Dbo exception model init: " << e.what());
        return 1;
    }

    // Try to create the schema (will fail if already exists).
    session.createTables();

    // Add an entry.
    Wt::Dbo::Transaction transaction(session);

    auto user = std::make_unique<db::User>();
    user->name = "Joe Doe";
    user->email = "joe.doe@gmail.com";
    user->passwd = "Secret";
    user->role = db::UserRole::GUEST;

    Wt::Dbo::ptr<db::User> userPtr = session.add(std::move(user));

    // Save session.
// try {
// Dbo::Transaction t(*dbSession_);
// dbSession_->flush();
// return t.commit();
// } catch (Dbo::Exception &e) {
// LOGCNT_ERROR("Dbo exception on TIQCon DB session save: " << e.what());
// return false;
// }

    // Cancel session.
// try {
// dbSession_->rereadAll();
// } catch (Dbo::Exception &e) {
// LOGCNT_ERROR("Dbo exception on TIQCon DB session cancel: " << e.what());
// return false;
// }

    return 0;
}
