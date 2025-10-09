#include <ui/gui.h>

GUI::GUI() {
    set_resizable(false);
    
    grid = new Gtk::Grid();
    
    path_widget = new PathInput(*this);
    option_widget = new Options();
    control_widget = new ControlPanel();
    progress_widget = new Progress();

    make_signals();
    progress_widget->hide();

    grid->attach(*path_widget, 0, 0, 2);
    grid->attach(*progress_widget, 0, 0, 2);
    grid->attach(*option_widget, 0, 1);
    grid->attach(*control_widget, 1, 1);

    file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/log", 0, 0);
    file_sink->set_pattern("[%H:%M:%S] [%^---%L---%$] [thread %t] %v");
    file_sink->set_level(spdlog::level::off);

    message_box_sink = std::make_shared<MessageSink>(
        [this](const std::string &msg){
            progress_widget->write_to_message_box(msg);
        }
    );
    message_box_sink->set_level(spdlog::level::debug);

    auto combined_logger = std::make_shared<spdlog::logger>("combined_logger", spdlog::sinks_init_list{file_sink, message_box_sink});
    combined_logger->set_level(spdlog::level::trace);

    spdlog::register_logger(combined_logger);
    spdlog::set_default_logger(combined_logger);

    set_child(*grid);
}

GUI::~GUI(){
    stop_progress_timer();
    if (worker) {
        if (model) {
            model->request_cancel();
        }
        if (worker->joinable()) {
            worker->join();
        }
        delete worker;
    }
    if (model) {
        delete model;
    }
    if (worker) delete worker;
    delete grid;
    delete path_widget;
    delete option_widget;
    delete control_widget;
}
void GUI::make_signals() {
    option_widget->get_log_level_select()->property_selected().signal_changed().connect(sigc::mem_fun(*this, &GUI::on_log_select));
    option_widget->get_display_logs_check()->signal_toggled().connect(sigc::mem_fun(*this, &GUI::on_message_box_check));
    control_widget->get_run_button()->signal_clicked().connect(sigc::mem_fun(*this, &GUI::on_run));
    control_widget->get_close_button()->signal_clicked().connect(sigc::mem_fun(*this, &GUI::close));
    control_widget->get_cancel_button()->signal_clicked().connect(sigc::mem_fun(*this, &GUI::on_cancel));
}

void GUI::on_log_select() {
    file_sink->set_level(spdlog::level::from_str(option_widget->get_selected_level().lowercase()));
}

void GUI::on_message_box_check() {
    progress_widget->set_display_logs(option_widget->get_display_logs_check()->get_active());
}

void GUI::on_run() {
    model = new Indexer();
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
        return;
    }
    run_start();
}

void GUI::on_cancel() {
    spdlog::info("Confirming cancellation");
    model->pause();

    auto confirm_dialog = new Gtk::MessageDialog(*this,
                                                "Are you sure you want to cancel?",
                                                false,
                                                Gtk::MessageType::QUESTION,
                                                Gtk::ButtonsType::OK_CANCEL,
                                                true
    );
    confirm_dialog->set_title("Cancel?");

    confirm_dialog->signal_response().connect([this, confirm_dialog](int response_id) {
        if(response_id == Gtk::ResponseType::OK) {
            model->request_cancel();
        }
        model->resume();
        confirm_dialog->hide();
        delete confirm_dialog;
    });

    confirm_dialog->show();
}

bool GUI::on_close_request() {
    if(worker && model) model->pause();

    auto confirm_dialog = new Gtk::MessageDialog(*this,
                                                "Are you sure you want to quit?",
                                                false,
                                                Gtk::MessageType::QUESTION,
                                                Gtk::ButtonsType::OK_CANCEL,
                                                true
    );
    confirm_dialog->set_title("Quit?");
    confirm_dialog->set_transient_for(*this);

    auto loop = Glib::MainLoop::create();
    int response = Gtk::ResponseType::CANCEL;
    confirm_dialog->signal_response().connect([&](int resp) {
        response = resp;
        loop->quit();
    });

    confirm_dialog->show();
    loop->run();

    confirm_dialog->hide();
    delete confirm_dialog;

    if(worker && model) model->resume();

    return response != Gtk::ResponseType::OK;
}

void GUI::run_start() {
    path_widget->hide();
    progress_widget->show();

    option_widget->set_sensitive(false);
    control_widget->switch_state(false);


    bool json = option_widget->get_json_check()->get_active();

    worker = new std::thread([this, json] () {
        model->run_all(json);

        Glib::signal_idle().connect_once(sigc::mem_fun(*this, &GUI::run_end));
    });

    start_progress_timer();
}

void GUI::run_end() {
    stop_progress_timer();

    if (worker) {
        if (worker->joinable()) worker->join();
        delete worker;
        worker = nullptr;
    }

    delete model;
    model = nullptr;

    path_widget->show();
    progress_widget->hide();

    option_widget->set_sensitive(true);
    control_widget->switch_state(true);
}

void GUI::start_progress_timer(unsigned int interval_ms) {
    if (progress_timeout_connection.connected()) return;

    progress_timeout_connection = Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &GUI::on_progress_timeout),
        interval_ms
    );
}

void GUI::stop_progress_timer() {
    if (progress_timeout_connection.connected()) {
        progress_timeout_connection.disconnect();
    }
}

bool GUI::on_progress_timeout() {
    auto pb = progress_widget->get_progress_bar();
    if(pb->get_fraction() >= 1.0f) return false;
    if (pb) {
        pb->set_fraction(model->get_progress());
    }

    return true;
}