#include <ui/elements/progress.h>

Progress::Progress() {
    log_message = Gtk::TextBuffer::create();

    progress_bar = new Gtk::ProgressBar();
    progress_bar->set_size_request(538, 34);
    progress_bar->set_margin(5);

    message_box = new Gtk::TextView();
    message_box->set_margin(5);
    message_box->set_size_request(538, 238);
    message_box->set_sensitive(false);
    message_box->set_visible(display_logs);
    message_box->set_buffer(log_message);

    attach(*progress_bar, 0, 0);
    attach(*message_box, 0, 1);
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

void Progress::write_to_message_box(const std::string& msg) {
    log_message->insert(log_message->end(), msg);
}