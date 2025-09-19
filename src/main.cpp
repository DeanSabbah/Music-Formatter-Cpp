#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>

#include "model/Indexer.h"

namespace fs = std::filesystem;

int main(int argc, char const *argv[]) {

    auto logger = spdlog::daily_logger_mt("logger", "logs/log");
    spdlog::set_default_logger(logger);

    spdlog::set_pattern("[%H:%M:%S] [%^---%L---%$] [thread %t] %v");
    spdlog::set_level(spdlog::level::info);

    Indexer* indexer = new Indexer(fs::current_path());
    
    try{
        indexer->set_base_path("C:/Eat/My/Ass");
    }
    catch(fs::filesystem_error fse){}
    
    std::cout<<indexer->get_base_path()<<std::endl;

    indexer->index_files();

    indexer->write_json();

    indexer->move_files();

    delete indexer;

    return 0;
}
