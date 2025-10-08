#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <thread>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/messagedialog.h>

#include <ui/elements/path_input.h>
#include <ui/elements/control_panel.h>
#include <ui/elements/options.h>
#include <ui/elements/progress.h>
#include <model/indexer.h>
#include <model/message_box_sink.h>

class GUI : public Gtk::Window {
public:
    GUI();
    ~GUI() override;

    //Member functions
    inline PathInput* get_path_widget() const { return path_widget; }
    inline ControlPanel* get_control_widget() const { return control_widget; }
    inline Options* get_option_widget() const { return option_widget; }

private:
    Indexer* model = nullptr;
    //Member functions 
    void make_signals();

    void run_start();
    void run_end();

    bool on_progress_timeout();
    void start_progress_timer(unsigned int interval_ms = 500);
    void stop_progress_timer();

    //Signal handlers:
    void on_log_select();
    void on_message_box_check();
    void on_run();
    void on_cancel();
  
    //Member widgets:
    Gtk::Grid*      grid;
    PathInput*      path_widget;
    ControlPanel*   control_widget;
    Options*        option_widget;
    Progress*       progress_widget;
    
    //Log sinks
    std::shared_ptr<spdlog::sinks::daily_file_sink_mt>  file_sink;
    std::shared_ptr<MessageSink>                        message_box_sink;

    // Member Variables
    sigc::connection progress_timeout_connection;
    std::thread* worker = nullptr;
};

#endif