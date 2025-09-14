#include <iostream>

#include "Indexer.h"

Indexer::Indexer(const fs::path& path) {
    set_base_path(path);
    music_index = new std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::pair<std::string, fs::path>>>>();
}

Indexer::~Indexer() {
    delete music_index;
}

void Indexer::set_base_path(const fs::path& path) {
    if(!fs::exists(path)){
        throw fs::filesystem_error(
        "Path not found",
        path,
        std::make_error_code(std::errc::no_such_file_or_directory)
        );
    }
    base_path = path;
}

std::string Indexer::generate_random_string(int len, unsigned long long seed = time(NULL)){
    srand(seed);

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string s;
    s.reserve(len);

    for (int i = 0; i < len; ++i) {
        s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return s;
}

void Indexer::check_permission() {
    std::string temp_dir = base_path.append(generate_random_string(20));
    try{
        fs::create_directory(temp_dir);
        fs::remove(temp_dir);
    }
    catch (fs::filesystem_error fse){
        throw fse;
    }
}

bool Indexer::is_supported_type(const fs::path& path){
    TagLib::FileRef f(path.c_str());
    return !f.isNull() && f.tag();
}

void Indexer::index_files() {
    for(const fs::directory_entry & entry : fs::directory_iterator(base_path)){
        if(is_supported_type(entry.path())){
            std::cout<<"Yay, "<<entry.path().extension()<<" is supported!"<<std::endl;
        }
        else{
            std::cout<<"Aww, "<<entry.path().extension()<<" isn't supported..."<<std::endl;
        }
    }
}

void Indexer::write_json() {
    check_permission();
}

void Indexer::move_files() {
    check_permission();
}