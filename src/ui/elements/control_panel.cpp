#include <ui/elements/control_panel.h>

ControlPanel::ControlPanel() : Gtk::Box(Gtk::Orientation::HORIZONTAL, 5) {
    set_valign(Gtk::Align::END);
    set_halign(Gtk::Align::END);
    set_margin(5);

    make_run_button();
    make_cancel_button();

    append(*run_button);
    append(*cancel_button);
}

ControlPanel::~ControlPanel(){
    delete run_button;
    delete cancel_button;
}

void ControlPanel::make_run_button() {
    run_button = new Gtk::Button("Run");

    run_button->set_size_request(118, -1);

    run_button->set_hexpand(false);
    run_button->set_vexpand(false);
    run_button->set_valign(Gtk::Align::END);
    run_button->set_halign(Gtk::Align::END);
}

void ControlPanel::make_cancel_button() {
    cancel_button = new Gtk::Button("Cancel");

    cancel_button->set_size_request(118, -1);

    cancel_button->set_hexpand(false);
    cancel_button->set_vexpand(false);
    cancel_button->set_valign(Gtk::Align::END);
    cancel_button->set_halign(Gtk::Align::END);
    cancel_button->set_sensitive(false);
}