#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>

#include <ui/elements/path_input.h>

class GUI : public Gtk::Window
{

public:
  GUI();
  ~GUI() override;

protected:
  //Signal handlers:

  //Member widgets:
  PathInput path_widget;
};

#endif