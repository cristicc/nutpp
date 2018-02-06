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

#ifndef NUTPP_WEBSERVER_NUTPPUI_H_
#define NUTPP_WEBSERVER_NUTPPUI_H_

#include <Wt/WApplication.h>

/*
 * Utility macros
 */
#define NUTPP_APP ((NutppUI *) wApp)

/*! \class NutppUI
 *  \brief The Web UI for the Nutpp application.
 *
 * The class implementing a web user interface for the Nutpp application.
 */
class NutppUI : public Wt::WApplication {
public:
    NutppUI(const Wt::WEnvironment &env);
    ~NutppUI();

private:
    // Specialization to handle application refresh
    void refresh() override;

    //! Counter used to limit max active sessions.
    static int session_cnt;
};

#endif /* NUTPP_WEBSERVER_NUTPPUI_H_ */
