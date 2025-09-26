#ifndef PATH_INPUT_H
#define PATH_INPUT_H

#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/filechoosernative.h>
#include <gtkmm/window.h>
#include <gtkmm/dialog.h>

class PathInput : public Gtk::Box {
public:
    PathInput(Gtk::Window& root);
    ~PathInput();

    inline Gtk::Button* get_path_button() const { return path_select; }
    inline Gtk::Entry* get_path_field() const { return path_field; }
private:
    // Builder
    void make_path_select();
    void make_path_field();
    // Member functions
    void on_path_select_click();
    void on_path_select_dialogue_end(int response_id);
    // Member Widgets
    Gtk::Button*                            path_select;
    Gtk::Entry*                             path_field;
    Glib::RefPtr<Gtk::FileChooserNative>    dir_dialogue;

    Gtk::Window&                      root_;
};

#endif