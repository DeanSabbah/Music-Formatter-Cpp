#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <random>

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

        inline fs::path get_base_path() const { return base_path; }
        inline int get_index_size() const { return index_size; }
        inline std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::pair<std::string, fs::path>>>>* get_music_index() const { return music_index; }
    private:
        // Member Variables
        fs::path base_path;

        std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::pair<std::string, fs::path>>>>* music_index;
        int index_size;

        // Helper Functions
        void check_permission();
        std::string generate_random_string(int len, unsigned long long seed);
        bool is_supported_type(const fs::path& path);
};