#include <ui/gui.h>

#include <iostream>

GUI::GUI() {
    set_resizable(false);
    
    grid = new Gtk::Grid();
    path_widget = new PathInput(*this);
    option_widget = new Options();
    control_widget = new ControlPanel();

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

#include <gtkmm/application.h>

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.gtkmm.example");

    //Shows the window and returns when it is closed.
    return app->make_window_and_run<GUI>(argc, argv);
}