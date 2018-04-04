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
 * @brief Widget to list patients.
 */

#ifndef NUTPP_WEBSERVER_PATIENTLISTVIEW_H_
#define NUTPP_WEBSERVER_PATIENTLISTVIEW_H_

#include <Wt/WTemplateFormView.h>
#include "storage/patient.h"

namespace nutpp {
namespace webserver {
/**
 * @brief Widget to edit patient information.
 */
class PatientListView : public Wt::WTemplateFormView {
public:
    /// Constructor.
    PatientListView();

    /// Loads/reloads patients from database.
    void loadPatients();

    /**
     * Set/reset search filter for patient names.
     *
     * @param[in] search Show only the items matching the filter.
     */
    void filter(std::string search = "");

    /// Returns the number of pages currently shown.
    int pageCount() const;

    /// Returns the number of items per page.
    int pageSize() const;

    /// Returns the zero-based index of current page.
    int currentPage() const;

    /**
     * @brief Changes the current page.
     * @param[in] page The zero-based index of the new page.
     */
    void setCurrentPage(int page);

    /**
     * @brief Signal emitted when page-related information was updated.
     *
     * Possible events: current page changed, the number of rows changed.
     */
    Wt::Signal<> &pageChanged() { return page_changed_; }

protected:
    /// Delay loading patients until rendering time.
    virtual void render(Wt::WFlags<Wt::RenderFlag> flags) override;

private:
    void viewPatient(const Wt::Dbo::ptr<storage::Patient> &patient);
    void editPatient(const Wt::Dbo::ptr<storage::Patient> &patient);
    void deletePatient(const Wt::Dbo::ptr<storage::Patient> &patient);

    static const int kDefaultPageSize = 2;
    std::string search_filter_;
    int page_size_;
    int crt_page_;  // Zero-based page index.
    int total_pages_;
    std::unique_ptr<Wt::WDialog> dialog_;
    std::unique_ptr<Wt::WMessageBox> msgbox_;
    Wt::Signal<> page_changed_;
};
} // namespace webserver
} // namespace nutpp
#endif // NUTPP_WEBSERVER_PATIENTLISTVIEW_H_
