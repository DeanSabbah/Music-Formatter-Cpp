#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>

#include <ui/elements/path_input.h>
#include <ui/elements/control_panel.h>
#include <ui/elements/options.h>

class GUI : public Gtk::Window {
public:
    GUI();
    ~GUI() override;

protected:
    //Signal handlers:

    //Widget builders
    //Member widgets:
    Gtk::Grid*      grid;
    PathInput*      path_widget;
    ControlPanel*   control_widget;
    Options*        option_widget;
};

#endif