#include <spdlog/spdlog.h>

#include <model/Indexer.h>

Indexer::Indexer(const fs::path& path) {
    init(path);
}

Indexer::Indexer() {
    init(fs::current_path());    
}

void Indexer::init(const fs::path& path) {
    init_logger();
    set_base_path(path);
    music_index = new std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::pair<std::string, fs::path>>>>();
}

Indexer::~Indexer() {
    delete music_index;
}

void Indexer::init_logger(const spdlog::level::level_enum& level){
    init_logger();
    spdlog::set_level(level);
}

void Indexer::init_logger() {
    try {
        logger = spdlog::get("logger");
        if (!logger) {
            logger = spdlog::stdout_color_mt("logger");
            spdlog::set_default_logger(logger);
        }
    }
    catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }
    
    spdlog::set_pattern("[%H:%M:%S] [%^---%L---%$] [thread %t] %v");
    spdlog::set_level(spdlog::level::info);
}

void Indexer::set_base_path(const fs::path& path) {
    if(!fs::is_directory(path) || !fs::exists(path)){
        logger->warn("Directory {} does not exist or is not a directory", path.string());
        throw fs::filesystem_error(
        "Directory not found",
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
    std::string temp_dir = base_path.string() + generate_random_string(20);
    logger->info("Checking permission");
    try{
        fs::create_directory(temp_dir);
        fs::remove(temp_dir);
    }
    catch (const fs::filesystem_error& fse){
        if(fse.code() == std::errc::permission_denied) logger->error("Permission denied");
        if(fse.code() == std::errc::file_exists) logger->error("File already exists");
        throw fse;
    }
}

bool Indexer::is_supported_type(const TagLib::FileRef& f) const {
    return !f.isNull() && f.tag();
}

void Indexer::index_files() {
    logger->info("Starting file indexing");
    for(const fs::directory_entry & entry : fs::directory_iterator(base_path)) {
        TagLib::FileRef f;
        try{
            if(!fs::exists(entry.path())) throw fs::filesystem_error("File no longer exists", entry.path(), std::make_error_code(std::errc::no_such_file_or_directory));
            f = TagLib::FileRef(entry.path().c_str());
        }
        catch(const fs::filesystem_error& e){
            if(e.code() == std::errc::no_such_file_or_directory) {
                logger->warn("File not found, continuing");
                logger->debug("Missing file: {}", entry.path().string());
                continue;
            }
        }
        if(!is_supported_type(f)) continue;

        // Get relevant information from file
        std::string artist = f.tag()->artist().toCString(), album = f.tag()->album().toCString(), track = f.tag()->title().toCString();
        // Create map for artist
        if(!music_index->count(artist)) {
            logger->debug("Adding {} to artist index", artist);
            music_index->emplace(artist, std::unordered_map<std::string, std::vector<std::pair<std::string, fs::path>>>());
        }
        // Add album vector to artist map
        if(!music_index->at(artist).count(album)) {
            logger->debug("Adding {} album to {} index", album, artist);
            music_index->at(artist).emplace(album, std::vector<std::pair<std::string, fs::path>>());
        }
        logger->debug("Adding {} track to {} album", track, album);
        music_index->at(artist).at(album).push_back(std::pair(track, entry.path()));
    }
}

void Indexer::write_json() {
    check_permission();

    std::ofstream json("music_index.json");
    json<<"{";
    
    for(auto artistIt = music_index->cbegin(); artistIt != music_index->cend(); ++artistIt){
        const auto& artist = artistIt->first;
        const auto& albums = artistIt->second;

        bool lastArtist = std::next(artistIt) == music_index->cend();

        json<<"\n  \""<<artist<<"\":\n  {\n";

        for(auto albumIt = albums.cbegin(); albumIt != albums.cend(); ++albumIt){
            const auto& album = albumIt->first;
            const auto& tracks = albumIt->second;

            bool lastAlbum = std::next(albumIt) == albums.cend();

            json<<"    \""<<album<<"\":\n    [\n";

            for(const auto& track : tracks){
                json<<"      \"";
                for(char c : track.first){
                    json<<(c == '\031' ? '\'' : c);
                }
                json<<(&track == &tracks.back() ? "\"\n" : "\",\n");
            }
            json<<(lastAlbum ? "    ]\n" : "    ],\n");
        }
        json<<(lastArtist ? "  }\n" : "  },");
    }
    json<<"}"<<std::endl;

    json.close();
}

void Indexer::move_files() {
    check_permission();

    logger->info("Moving files");
    
    for(const auto& artist : *music_index){
        for(const auto& album : artist.second){
            const std::string& artist_title = artist.first, album_title = album.first;
            for(const auto& track : album.second){
                const fs::path& track_path = track.second;
                fs::path new_path = fs::path(base_path / artist_title / album_title);
                logger->debug("Moving from >>" + track_path.string() + "<< to >>" + new_path.string() + "<<");
                try{
                    fs::create_directories(new_path);
                    fs::copy_file(track_path, new_path / track_path.filename());
                    fs::remove(track_path);
                }
                catch(fs::filesystem_error fse){
                    logger->critical("Error moving files");
                    logger->critical(fse.what());
                    throw fse;
                }
            }
        }
    }
}