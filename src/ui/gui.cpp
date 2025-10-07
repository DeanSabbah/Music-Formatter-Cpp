#include <ui/gui.h>

#include <iostream>

GUI::GUI() {
    model = new Indexer();
    set_resizable(false);
    grid = new Gtk::Grid();
    path_widget = new PathInput(*this);
    option_widget = new Options();
    control_widget = new ControlPanel();
    progress_widget = new Progress();

    make_signals();

    grid->attach(*path_widget, 0, 0, 2);
    grid->attach(*option_widget, 0, 1);
    grid->attach(*control_widget, 1, 1);

    set_child(*grid);
}

GUI::~GUI(){
    delete grid;
    delete path_widget;
    delete option_widget;
    delete control_widget;
}
void GUI::make_signals() {
    option_widget->get_display_logs_check()->signal_toggled().connect(sigc::mem_fun(*this, &GUI::on_message_box_check));
    control_widget->get_run_button()->signal_clicked().connect(sigc::mem_fun(*this, &GUI::on_run));
}

void GUI::on_log_select() {
    // TODO: Implement logs
}

void GUI::on_message_box_check() {
    progress_widget->set_display_logs(option_widget->get_display_logs_check()->get_active());
}

void GUI::on_run() {
    try {
        model->set_base_path(path_widget->get_inputed_path().c_str());
    }
    catch (fs::filesystem_error& e){
        auto error_dialog = new Gtk::MessageDialog(*this,
                                                  e.what(),
                                                  false,
                                                  Gtk::MessageType::ERROR,
                                                  Gtk::ButtonsType::OK,
                                                  true);
        error_dialog->set_title("Path Not Found");
        
        error_dialog->signal_response().connect([error_dialog](int response_id) {
            error_dialog->hide();
            delete error_dialog;
        });
        
        error_dialog->show();
    }
}