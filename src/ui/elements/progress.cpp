#include <ui/elements/progress.h>

Progress::Progress() {
    progress_bar = new Gtk::ProgressBar();
    message_box = new Gtk::Text();
    message_box->set_visible(display_logs);

    attach(*progress_bar, 0, 0);
    attach(*message_box, 1, 0);
}

Progress::Progress(bool display_logs_) : display_logs(display_logs_) {
    Progress();
}

Progress::~Progress() {
    delete progress_bar;
    delete message_box;
}

void Progress::set_display_logs(bool value) {
    display_logs = value;

    message_box->set_visible(display_logs);
}