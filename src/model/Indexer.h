#ifndef INDEXER_H
#define INDEXER_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <filesystem>
#include <random>
#include <iostream>
#include <fstream>

// TagLib import(s)
#include <taglib/fileref.h>

namespace fs = std::filesystem;

class Indexer {
    public:
        Indexer(const fs::path&);
        ~Indexer();

        // meat
        void index_files();
        void write_json();
        void move_files();

        void set_base_path(const fs::path& path);
        
        void init_logger();

        inline fs::path get_base_path() const { return base_path; }
        inline int get_index_size() const { return index_size; }
        inline auto get_music_index() const { return music_index; }
    private:
        // Member Variables
        fs::path base_path;
        std::shared_ptr<spdlog::logger> logger;
        
        // Map containing artists -> map containing albums -> vector containing tracks (and track paths)
        std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::pair<std::string, fs::path>>>>* music_index;
        int index_size;

        // Helper Functions
        void check_permission();
        std::string generate_random_string(int len, unsigned long long seed);
        bool is_supported_type(const TagLib::FileRef& f) const;
};

#endif