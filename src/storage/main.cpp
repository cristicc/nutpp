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

#include "db_session.h"
#include "user.h"
#include "util/log.h"
#include "util/log_initializer.h"

#include <iostream>

LOGNUTPP_LOGGER_STORAGE;

namespace storage = nutpp::storage;
namespace dbo = Wt::Dbo;

// Utility to perform database operations.
int main(int /*argc*/, char ** /*argv*/)
{
    // Initialize logging.
    nutpp::util::LogInitializer log_init(log4cplus::DEBUG_LOG_LEVEL);

    std::string log_file;
    if (!log_init.configure("log4cplus.properties", ".")) {
        std::cerr << "Failed to initialize logging" << std::endl;
    }

    LOGNUTPP_INFO("Initializing db model");
    storage::DbModel model("test.db", 1);

    // Create DB.
    int users;
    if (!model.createSchema(users)) {
        LOGNUTPP_FATAL("Database corrupted or not accessible");
        return 1;
    }

    storage::DbSession s1(model);
    LOGNUTPP_DEBUG("DB SQL: " << s1.getDboSession().tableCreationSql());

    // Add a few users.
    try {
        for (int i = 0; i < 2; i++) {
            auto user = std::make_unique<storage::User>();
            user->language = "ro";
            s1.add(std::move(user));
        }

        s1.commit();
    } catch (const std::exception &e) {
        LOGNUTPP_ERROR("Failed to add users: " << e.what());
    }

    // Verify
    try {
        storage::DbSession s2(model);
        int count = s2.getDboSession().query<int>(
            std::string("select count(1) from ") + storage::User::kTableName);
        LOGNUTPP_INFO("Total users: " << count);
    } catch (const std::exception &e) {
        LOGNUTPP_ERROR("Failed to verify total users: " << e.what());
    }

    return 0;
}
