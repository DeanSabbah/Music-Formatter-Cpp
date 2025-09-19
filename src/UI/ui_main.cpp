#include "ui_main.h"
#include <iostream>

HelloWorld::HelloWorld()
: m_button("Hello World")   // creates a new button with label "Hello World".
{
  // Sets the margin around the button.
  m_button.set_margin(10);

  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  m_button.signal_clicked().connect(sigc::mem_fun(*this,
              &HelloWorld::on_button_clicked));

  // This packs the button into the Window (a container).
  set_child(m_button);
}

HelloWorld::~HelloWorld()
{
}

void HelloWorld::on_button_clicked()
{
  std::cout << "Hello World" << std::endl;
}

#include <gtkmm/application.h>

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("org.gtkmm.example");

  //Shows the window and returns when it is closed.
  return app->make_window_and_run<HelloWorld>(argc, argv);
}