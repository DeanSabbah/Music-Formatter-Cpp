#include <ui/ui_main.h>
#include <ui/elements/path_input.h>

#include <iostream>

GUI::GUI()
: path_widget(*this) {
  set_child(path_widget);
}

GUI::~GUI(){}

#include <gtkmm/application.h>

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("org.gtkmm.example");

  //Shows the window and returns when it is closed.
  return app->make_window_and_run<GUI>(argc, argv);
}