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

#include "patient_list_view.h"

#include "nutpp_ui.h"
#include "patient_form_view.h"
#include "auth/login_session.h"
#include "storage/db_session.h"
#include "util/log.h"

#include <Wt/WDialog.h>
#include <Wt/WMessageBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>

LOGNUTPP_LOGGER_WS;

namespace nutpp {
namespace webserver {
/**
 * @brief Navigation bar.
 */
class PagingBar : public Wt::WContainerWidget {
public:
    /// C-tor.
    PagingBar(PatientListView *view);

private:
    void update();

    PatientListView *view_;
    Wt::WPushButton *prev_button_, *next_button_, *first_button_, *last_button_;
    Wt::WText *current_;
};

/**
 * @brief An item in the patients list.
 */
class PatientListItem : public Wt::WTemplate {
public:
    /// User action types.
    enum class Action {
        VIEW,
        EDIT,
        DELETE
    };

    /// C-tor.
    PatientListItem(const Wt::Dbo::ptr<storage::Patient> &patient, int index);

    /// Override.
    virtual void resolveString(const std::string &var_name,
                               const std::vector<Wt::WString> &args,
                               std::ostream &result) override;

    /// User action image clicked.
    Wt::Signal<Action> &actionClicked() { return action_clicked_; }

protected:
    /// Override.
    virtual void renderTemplate(std::ostream &result) override;

private:
    Wt::Dbo::ptr<storage::Patient> patient_;
    int index_;
    Wt::Signal<Action> action_clicked_;
};

// C-tor.
PagingBar::PagingBar(PatientListView *view)
    : view_(view)
{
    setStyleClass("Wt-pagingbar");

    first_button_ = addWidget(
        std::make_unique<Wt::WPushButton>(
            tr("Wt.WAbstractItemView.PageBar.First")));
    first_button_->clicked().connect(
        [=]() { view_->setCurrentPage(0); });

    prev_button_ = addWidget(
        std::make_unique<Wt::WPushButton>(
            tr("Wt.WAbstractItemView.PageBar.Previous")));
    prev_button_->clicked().connect(
        [=]() { view_->setCurrentPage(view_->currentPage() - 1); });

    current_ = addWidget(std::make_unique<Wt::WText>());

    next_button_ = addWidget(
        std::make_unique<Wt::WPushButton>(
            tr("Wt.WAbstractItemView.PageBar.Next")));
    next_button_->clicked().connect(
        [=]() { view_->setCurrentPage(view_->currentPage() + 1); });

    last_button_ = addWidget(
        std::make_unique<Wt::WPushButton>(
            tr("Wt.WAbstractItemView.PageBar.Last")));
    last_button_->clicked().connect(
        [=]() { view_->setCurrentPage(view_->pageCount() - 1); });

    view_->pageChanged().connect(this, &PagingBar::update);

    update();
}

// Update.
void PagingBar::update()
{
    first_button_->setDisabled(view_->currentPage() < 1);
    prev_button_->setDisabled(view_->currentPage() < 1);

    next_button_->setDisabled(view_->currentPage() == view_->pageCount() - 1);
    last_button_->setDisabled(view_->currentPage() == view_->pageCount() - 1);

    current_->setText(
        Wt::WString::tr("Wt.WAbstractItemView.PageIOfN")
        .arg(view_->currentPage() + 1).arg(view_->pageCount()));
}

// C-tor.
PatientListItem::PatientListItem(
    const Wt::Dbo::ptr<storage::Patient> &patient, int index)
    : patient_(patient),
    index_(index)
{
    addFunction("tr", &WTemplate::Functions::tr);
    setTemplateText(tr("nutpp.ws.template.list-patient-item"));

    auto name = bindWidget("name", std::make_unique<Wt::WText>(patient_->name));
    name->clicked().connect([=]() { action_clicked_.emit(Action::VIEW); });

    auto view = bindWidget(
        "view-button", std::make_unique<Wt::WContainerWidget>());
    view->setInline(true);
    view->setStyleClass("glyphicon glyphicon-list-alt");
    view->clicked().connect([=]() { action_clicked_.emit(Action::VIEW); });

    auto edit = bindWidget(
        "edit-button", std::make_unique<Wt::WContainerWidget>());
    edit->setInline(true);
    edit->setStyleClass("glyphicon glyphicon-pencil");
    edit->clicked().connect([=]() { action_clicked_.emit(Action::EDIT); });

    auto del = bindWidget(
        "delete-button", std::make_unique<Wt::WContainerWidget>());
    del->setStyleClass("glyphicon glyphicon-trash");
    del->setInline(true);
    del->clicked().connect([=]() { action_clicked_.emit(Action::DELETE); });
}

// Override.
void PatientListItem::renderTemplate(std::ostream &result)
{
    try {
        auto t = NUTPP_LOGIN.dbSession().createTransaction();
        WTemplate::renderTemplate(result);

        // Remove patient object from memory.
        patient_.purge();
        t->commit();
    } catch (const std::exception &e) {
        LOGNUTPP_ERROR("Failed to render patient list item: " << e.what());
    }
}

// Override.
void PatientListItem::resolveString(
    const std::string &var_name,
    const std::vector<Wt::WString> &args,
    std::ostream &result)
{
    if (var_name == "index") {
        format(result, std::to_string(index_));
    } else if (var_name == "email") {
        format(result, patient_->email);
    } else if (var_name == "phone") {
        format(result, patient_->phone_no);
    } else if (var_name == "last_activity") {
        format(result, patient_->modif_time.toString(
                   Wt::WLocale::currentLocale().dateTimeFormat()));
    } else {
        WTemplate::resolveString(var_name, args, result);
    }
}

// C-tor.
PatientListView::PatientListView()
    : page_size_(kDefaultPageSize),
    crt_page_(0),
    total_pages_(0)
{
    setTemplateText(tr("nutpp.ws.template.list-patient"));

    auto but = bindWidget(
        "add-pacient-button",
        std::make_unique<Wt::WPushButton>(
            Wt::WString::tr("nutpp.patient.add-new")));
    but->clicked().connect(
        [=]() {
            editPatient(Wt::Dbo::ptr<storage::Patient>(
                            std::make_unique<storage::Patient>()));
        });

    bindWidget("nav-bar", std::make_unique<PagingBar>(this));
}

// Load/reload data.
void PatientListView::loadPatients()
{
    LOGNUTPP_DEBUG("Loading patients using filter '" << search_filter_ << "'");

    // Previous container will be removed if set.
    auto list
        = bindWidget("contents", std::make_unique<Wt::WContainerWidget>());

    try {
        auto t = NUTPP_LOGIN.dbSession().createTransaction();

        storage::Patients patients
            = search_filter_.empty() ? NUTPP_LOGIN.user()->patients.find()
              : NUTPP_LOGIN.user()->patients.find()
              .where("name LIKE ?").bind("%" + search_filter_ + "%");

        total_pages_ = std::ceil(
            static_cast<float>(patients.size()) / page_size_);
        if (crt_page_ >= total_pages_) {
            crt_page_ = total_pages_ - 1;
        }

        int item_start = crt_page_ * page_size_;
        int item_end = item_start + page_size_ - 1;
        int index = -1;

        for (auto patient : patients) {
            ++index;
            if (index < item_start) {
                continue;
            }

            if (index > item_end) {
                break;
            }

            auto item = list->addWidget(
                std::make_unique<PatientListItem>(patient, index + 1));
            item->actionClicked().connect(
                [=](PatientListItem::Action op) {
                    if (op == PatientListItem::Action::VIEW) {
                        viewPatient(patient);
                    } else if (op == PatientListItem::Action::EDIT) {
                        editPatient(patient);
                    } else if (op == PatientListItem::Action::DELETE) {
                        deletePatient(patient);
                    }
                });
        }
    } catch (const std::exception &e) {
        LOGNUTPP_ERROR("Failed to load patients list: " << e.what());
    }

    // Update nav bar.
    page_changed_.emit();
}

// Change search filter.
void PatientListView::filter(std::string search)
{
    search_filter_ = search;
    crt_page_ = 0;
    loadPatients();
}

// Page count.
int PatientListView::pageCount() const
{
    return total_pages_;
}

// Page size.
int PatientListView::pageSize() const
{
    return page_size_;
}

// Crt page.
int PatientListView::currentPage() const
{
    return crt_page_;
}

// Change page.
void PatientListView::setCurrentPage(int page)
{
    crt_page_ = page;
    loadPatients();
}

// Delay loading from db.
void PatientListView::render(Wt::WFlags<Wt::RenderFlag> flags)
{
    if (!resolveWidget("contents")) {
        loadPatients();
    }

    WTemplateFormView::render(flags);
}

// View detailed pacient info.
void PatientListView::viewPatient(const Wt::Dbo::ptr<storage::Patient> &patient)
{}

// Show patient editor.
void PatientListView::editPatient(const Wt::Dbo::ptr<storage::Patient> &patient)
{
    bool is_new = patient.isTransient();

    dialog_ = std::make_unique<Wt::WDialog>(
        tr(is_new ? "nutpp.patient.add-new" : "nutpp.patient.edit"));

    auto form = std::make_unique<PatientFormView>(patient);
    form->saved().connect(
        [=]() {
            if (is_new) {
                // Show last page when adding a new item.
                crt_page_ = total_pages_;
            }

            loadPatients();
        });

    dialog_->contents()->addWidget(std::move(form));
    dialog_->footer()->hide();
    dialog_->contents()->childrenChanged().connect([=]() { dialog_.reset(); });
    dialog_->show();
}

// Show patient delete confirmation dialog.
void PatientListView::deletePatient(
    const Wt::Dbo::ptr<storage::Patient> &patient)
{
    msgbox_ = std::make_unique<Wt::WMessageBox>(
        tr("nutpp.patient.delete"), tr("nutpp.patient.delete-confirm"),
        Wt::Icon::Question,
        Wt::StandardButton::Ok | Wt::StandardButton::Cancel);

    msgbox_->buttonClicked().connect(
        [=](Wt::StandardButton but) {
            if (but == Wt::StandardButton::Ok) {
                bool done = false;

                try {
                    storage::DbSession s(patient);
                    Wt::Dbo::ptr<storage::Patient> p = patient;
                    p.remove();
                    done = s.commit();
                } catch (const std::exception &e) {
                    LOGNUTPP_ERROR("Failed to delete patient: " << e.what());
                }

                if (done) {
                    loadPatients();
                }
            }

            msgbox_.reset();
        });

    msgbox_->show();
}
} // namespace webserver
} // namespace nutpp
