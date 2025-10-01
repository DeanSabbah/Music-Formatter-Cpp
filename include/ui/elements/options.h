#ifndef OPTIONS_H
#define OPTIONS_H

#include <gtkmm/grid.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/dropdown.h>
#include <gtkmm/stringlist.h>

class Options : public Gtk::Grid {
public:
    Options();
    ~Options();

    inline Gtk::CheckButton* get_json_check() const { return json_check; }
    inline Gtk::CheckButton* get_display_logs_check() const { return display_logs_check; }
    inline Gtk::DropDown*    get_log_level_select() const { return log_level_select; }
private:
    void build_log_level_select();

    Gtk::CheckButton*   json_check;
    Gtk::CheckButton*   display_logs_check;
    Gtk::DropDown*      log_level_select;

    Glib::RefPtr<Gio::ListModel> log_options;
    const std::vector<Glib::ustring> levels;
};

#endif