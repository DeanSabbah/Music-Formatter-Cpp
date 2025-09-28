#include <ui/ui_main.h>

#include <iostream>

GUI::GUI() {
    grid = new Gtk::Grid();
    path_widget = new PathInput(*this);
    control_widget = new ControlPanel();

    grid->attach(*path_widget, 0, 0, 2);
    grid->attach(*control_widget, 1, 1);

    set_child(*grid);
}

GUI::~GUI(){}

#include <gtkmm/application.h>

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.gtkmm.example");

    //Shows the window and returns when it is closed.
    return app->make_window_and_run<GUI>(argc, argv);
}