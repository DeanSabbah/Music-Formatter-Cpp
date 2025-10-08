#ifndef PROGRESS_H
#define PROGRESS_H

#include <gtkmm/grid.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/textview.h>

class Progress : public Gtk::Grid {
public:
    Progress();
    Progress(bool display_logs_);
    ~Progress();

    void set_display_logs(bool);
    void write_to_message_box(const std::string& msg);

    inline bool get_display_logs() const { return display_logs; }
    inline Gtk::ProgressBar* get_progress_bar() const { return progress_bar; }
    inline Gtk::TextView* get_message_box() const { return message_box; }
private:
    bool                display_logs = false;
    float               progress = 0;

    Glib::RefPtr<Gtk::TextBuffer>    log_message;

    Gtk::ProgressBar*   progress_bar;
    Gtk::TextView*      message_box;
};

#endif