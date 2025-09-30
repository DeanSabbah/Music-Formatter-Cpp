#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <gtkmm/box.h>
#include <gtkmm/button.h>

class ControlPanel : public Gtk::Box {
public:
    ControlPanel();
    ~ControlPanel();

    inline Gtk::Button* get_run_button() const { return run_button; }
    inline Gtk::Button* get_cancel_button() const { return cancel_button; }
private:
    void make_run_button();
    void make_cancel_button();

    Gtk::Button* run_button;
    Gtk::Button* cancel_button;
};

#endif