#include <ui/elements/options.h>
#include <iostream>

Options::Options() : levels({ "Off", "Debug", "Info", "Warning", "Error", "Critical"}) {
    set_margin(5);
    set_halign(Gtk::Align::START);
    set_valign(Gtk::Align::END);
    
    json_check = new Gtk::CheckButton("Output JSON?");
    display_logs_check = new Gtk::CheckButton("Display Logs?");

    build_log_level_select();

    attach(*json_check, 0, 0);
    attach(*display_logs_check, 0, 1);
    attach(*log_level_select, 0, 2);
}

Options::~Options() {
    delete json_check;
    delete display_logs_check;
    delete log_level_select;
}

Glib::ustring Options::get_selected_level() {
    return levels[log_level_select->get_selected()];
}

void Options::build_log_level_select() {
    log_options = Gtk::StringList::create(levels);
    
    log_level_select = new Gtk::DropDown(log_options);
}