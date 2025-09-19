#include <iostream>

#include <model/Indexer.h>

#include <spdlog/spdlog.h>

#include <spdlog/sinks/daily_file_sink.h>

namespace fs = std::filesystem;

int main(int argc, char const *argv[]) {

    auto logger = spdlog::daily_logger_mt("logger", "logs/log");

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
