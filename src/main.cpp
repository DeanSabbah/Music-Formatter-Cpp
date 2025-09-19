#include <iostream>

#include <model/Indexer.h>

#include <spdlog/spdlog.h>

#include <argparse/argparse.hpp>

namespace fs = std::filesystem;

int main(int argc, char const *argv[]) {
    argparse::ArgumentParser program("music_formatter_cli", "0.0.1");

    program.add_argument("-p", "--path")
        .help("Set path that the program will target. Default is directory the program is located in.")
        .default_value(fs::current_path());
    
    program.add_argument("-l", "--log_level")
        .help("Set what level of logs to log. Default is off")
        .default_value("off")
        .choices("off", "trace", "debug", "info", "warning", "error", "critical");

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

    if(program.is_used("--path")){
        indexer = new Indexer(program.get<std::string>("--path"), level);
    }
    else{
        indexer = new Indexer(level);
    }

    indexer->index_files();

    indexer->write_json();

    indexer->move_files();

    delete indexer;

    return 0;
}
