#include <iostream>

#include "model/Indexer.h"

namespace fs = std::filesystem;

int main(int argc, char const *argv[]) {
    
    Indexer* indexer = new Indexer(fs::current_path());
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::pair<std::string, fs::path>>>>* index = indexer->get_music_index();

    // try{
    //     indexer->set_base_path("C:/Eat/My/Ass");
    // }
    // catch(fs::filesystem_error fse){
    //     std::cout<<"oops"<<"\n";
    //     std::cout<<fse.what()<<"\n";
    //     std::cout<<fse.code()<<std::endl;
    //     return fse.code().value();
    // }


    std::cout<<indexer->get_base_path()<<std::endl;


    indexer->index_files();

    delete indexer;

    return 0;
}
