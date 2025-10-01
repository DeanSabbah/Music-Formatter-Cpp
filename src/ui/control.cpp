#include <ui/control.h>

#include <gtkmm/application.h>

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("music.formatter");

    //Shows the window and returns when it is closed.
    return app->make_window_and_run<GUI>(argc, argv);
}