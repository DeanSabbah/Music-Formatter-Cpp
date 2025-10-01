#ifndef PROGRESS_H
#define PROGRESS_H

#include <gtkmm/grid.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/text.h>

class Progress : Gtk::Grid {
public:
    Progress();
    Progress(bool display_logs_);
    ~Progress();

    void set_display_logs(bool);

    inline bool get_display_logs() const { return display_logs; }
    inline Gtk::ProgressBar* get_progress_bar() const { return progress_bar; }
    inline Gtk::Text* get_message_box() const { return message_box; }
private:
    bool                display_logs = false;
    float               progress = 0;

    Gtk::ProgressBar*   progress_bar;
    Gtk::Text*          message_box;
};

#endif