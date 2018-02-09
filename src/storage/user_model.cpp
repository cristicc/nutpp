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

#include "user_model.h"

namespace nutpp {
namespace storage {
UserModel::UserModel()
{
//    try {
//        model_ = new Wt::Dbo::QueryModel<Wt::Dbo::ptr<User>>(this);
//        model_->setQuery(dbSession_->find<User>());
//        model_->setBatchSize(50);

//        std::vector<const char *> visibleColumns = {
//            TIQInstrConfig::SerialNoColumn,
//            TIQInstrConfig::ModuleTypeColumn,
//            TIQInstrConfig::ModulesColumn
//        };
//        for (size_t i = 0; i < visibleColumns.size(); i++) {
//            std::string resID = userColumnResMsgID<TIQInstrConfig>(visibleColumns[i]);
//            instrConfigLightModel_->addColumn(visibleColumns[i],
//                    WString::tr(resID), ItemIsSelectable);
//        }

//    } catch (Wt::Dbo::Exception &e) {
//        LOGCNT_ERROR("Dbo exception on " << TIQInstrConfig::TableName
//                << " query light model creation: " << e.what());
//        delete instrConfigLightModel_;
//        return NULL;
//    }
}
} // namespace storage
} // namespace nutpp
