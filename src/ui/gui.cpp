#include <ui/gui.h>

#include <iostream>

GUI::GUI() {
    model = new Indexer();
    set_resizable(false);
    grid = new Gtk::Grid();
    path_widget = new PathInput(*this);
    option_widget = new Options();
    control_widget = new ControlPanel();
    progress_widget = new Progress();

    grid->attach(*path_widget, 0, 0, 2);
    grid->attach(*option_widget, 0, 1);
    grid->attach(*control_widget, 1, 1);

    set_child(*grid);
}

GUI::~GUI(){
    delete grid;
    delete path_widget;
    delete option_widget;
    delete control_widget;
}

void GUI::on_log_select() {
    // TODO: Implement logs
}

void GUI::on_message_box_check() {
    progress_widget->set_display_logs(option_widget->get_display_logs_check()->get_active());
}