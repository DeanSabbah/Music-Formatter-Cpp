#include <iostream>

#include "model/Indexer.h"

namespace fs = std::filesystem;

int main(int argc, char const *argv[]) {
    
    Indexer* indexer = new Indexer(fs::current_path());
    
    try{
        indexer->set_base_path("C:/Eat/My/Ass");
    }
    catch(fs::filesystem_error fse){
        std::cout<<"oops"<<"\n";
        std::cout<<fse.what()<<"\n";
        std::cout<<fse.code()<<std::endl;
    }


    std::cout<<indexer->get_base_path()<<std::endl;


    indexer->index_files();

    auto index = indexer->get_music_index();

    for(auto artist : *index){
        std::cout<<artist.first<<"\n";
        for(auto album : artist.second){
            std::cout<<"  "<<album.first<<"\n";
            for(auto track : album.second){
                std::cout<<"    Name: "<<track.first<<"\n";
                std::cout<<"    Path: "<<track.second<<"\n";
            }
        }
        std::cout<<std::endl;
    }

    delete indexer;

    return 0;
}
