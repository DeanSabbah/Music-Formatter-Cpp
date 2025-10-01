#include <iostream>

#include <model/indexer.h>
#include <ui/gui.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <argparse/argparse.hpp>

#include <gtkmm/application.h>

namespace fs = std::filesystem;

int main(int argc, char **argv) {
    if (argc == 1) {
        auto app = Gtk::Application::create("com.deansabbah.music_formatter");
        return app->make_window_and_run<GUI>(argc, argv);
    }

    argparse::ArgumentParser program("music_formatter_cli", "0.0.1");

    program.add_argument("-p", "--path")
        .help("Set path that the program will target")
        .default_value(fs::current_path().string());
    
    program.add_argument("-l", "--log_level")
        .help("Set what level of logs to log")
        .default_value("off")
        .choices("off", "trace", "debug", "info", "warning", "error", "critical");

    program.add_argument("-j", "--write_json")
        .help("Create a json file that contains the index of the new file structure")
        .default_value(false)
        .choices(true, false);

    program.add_argument("--verbose")
        .help("Print logs to console. Default is false")
        .default_value(false)
        .implicit_value(true)
        .choices(true, false);

    Indexer* indexer;

    try{
        program.parse_args(argc, argv);
    }
    catch(const std::exception& err){
        std::cout<<":("<<std::endl;
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    spdlog::level::level_enum level = spdlog::level::from_str(program.get<std::string>("--log_level"));

    auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/log", 0, 0);
    file_sink->set_pattern("[%H:%M:%S] [%^---%L---%$] [thread %t] %v");
    file_sink->set_level(level);

    if(program.get<bool>("--verbose")){
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("%H:%M:%S: %v");
        console_sink->set_level(spdlog::level::debug);

        auto combined_logger = std::make_shared<spdlog::logger>("combined_logger", spdlog::sinks_init_list{file_sink, console_sink});
        
        spdlog::register_logger(combined_logger);
        spdlog::set_default_logger(combined_logger);
    }
    else{
        auto file_logger = std::make_shared<spdlog::logger>("file_logger", file_sink);
        spdlog::register_logger(file_logger);
        spdlog::set_default_logger(file_logger);
    }

    indexer = new Indexer(program.get<std::string>("--path"));
    
    spdlog::info("Beggining process");

    indexer->index_files();

    if(program.get<bool>("--write_json")) indexer->write_json();

    indexer->move_files();

    spdlog::info("Process completed successfully");

    delete indexer;

    return 0;
}
