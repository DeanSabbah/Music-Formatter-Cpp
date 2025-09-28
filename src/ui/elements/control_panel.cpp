#include <ui/elements/control_panel.h>

ControlPanel::ControlPanel() {
    run_button = new Gtk::Button("Run");
    run_button->set_margin(5);

    cancel_button = new Gtk::Button("Cancel");
    cancel_button->set_margin(5);
    cancel_button->set_sensitive(false);

    append(*run_button);
    append(*cancel_button);
}

ControlPanel::~ControlPanel(){
    delete run_button;
    delete cancel_button;
}