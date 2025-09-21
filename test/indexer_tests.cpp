#include <iostream>
#include <string>
#include <unordered_set>

#include <model/Indexer.h>

#include <taglib/fileref.h>

#include <catch2/catch_test_macros.hpp>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

Indexer* indexer;

TEST_CASE("Basepath testing") {
    indexer = new Indexer();
    fs::path expected_path = fs::current_path();
    
    SECTION("default path"){
        REQUIRE(indexer->get_base_path() == expected_path);
    }
    SECTION("change to new path") {
        REQUIRE_THROWS_AS(indexer->set_base_path("Eat/My/Ass"), fs::filesystem_error);
        
        fs::path new_path = fs::path(fs::current_path() / "test");
        
        fs::create_directories(new_path);
        indexer->set_base_path(new_path);
        
        REQUIRE(indexer->get_base_path() == new_path);

        fs::remove(new_path);
    }

    delete indexer;
}

TEST_CASE("Index Functions") {
    indexer = new Indexer();

    std::unordered_set<std::string> artists(2);
    std::unordered_set<std::string> albums(4);
    std::unordered_set<std::string> tracks(12);
    std::unordered_set<fs::path> track_paths(12);

    // Make files for testing
    for(int i = 0; i < 2; i++){
        std::string artist_name = std::to_string(i);
        artists.emplace(artist_name);
        for(int j = 0; j < 2; j++){
            std::string album_name = std::to_string(i) + std::to_string(j);
            albums.emplace(album_name);
            for(int k = 0; k < 3; k++){
                std::string file_name = "file" + std::to_string(i) + std::to_string(j) + std::to_string(k) + ".mp3";
                std::string track_name = std::to_string(i) + std::to_string(j) + std::to_string(k);
                tracks.emplace(track_name);
                track_paths.emplace(fs::path(fs::current_path() / file_name));

                std::ofstream file(file_name);
                file.close();

                TagLib::FileRef f(file_name.c_str());

                
                f.tag()->setAlbum(album_name);
                f.tag()->setArtist(artist_name);
                f.tag()->setTitle(track_name);
                f.save();
            }
        }
    }

    SECTION("Index Files") {
        indexer->index_files();
        auto index = indexer->get_music_index();

        REQUIRE_FALSE(index->size() == 0);

        for(auto artist : *index){
            REQUIRE(artists.contains(artist.first));
            for(auto album : artist.second){
                REQUIRE((albums.contains(album.first) && album.first.starts_with(artist.first)));
                for(auto track : album.second){
                    REQUIRE((tracks.contains(track.first) && track_paths.contains(track.second) && track.first.starts_with(album.first)));
                }
            }
        }
    }

    SECTION("Write JSON"){
        indexer->index_files();
        indexer->write_json();

        REQUIRE(fs::exists("music_index.json"));

        std::ifstream in(indexer->get_base_path() / "music_index.json");
        REQUIRE(in.good());
        nlohmann::json j;
        in >> j;
        REQUIRE(j.is_object());
     
        auto index = indexer->get_music_index();
        for (const auto& [artist, albums] : *index) {
            REQUIRE(j.contains(artist));
            const auto& j_albums = j.at(artist);
            REQUIRE(j_albums.is_object());
            for (const auto& [album, tracks] : albums) {
                REQUIRE(j_albums.contains(album));
                const auto& j_tracks = j_albums.at(album);
                REQUIRE(j_tracks.is_array());
                for (const auto& [track, path] : tracks) {
                    bool found = false;
                    for (const auto& item : j_tracks) {
                        if (fs::path(item.get<std::string>()) == track) {
                            found = true;
                            break;
                        }
                    }
                    REQUIRE(found);
                }
            }
        }
        fs::remove("music_index.json");
    }

    SECTION("Move files"){
        indexer->index_files();
        auto index = indexer->get_music_index();
        indexer->move_files();

        for(const auto& file : track_paths){
            REQUIRE_FALSE(fs::exists(file));
        }

        for(const auto& [artist, albums] : *index){
            for(const auto& [album, tracks] : albums){
                for(const auto& [title, path] : tracks){
                    REQUIRE(fs::exists(indexer->get_base_path() / artist / album / path.filename()));
                }
            }
        }
    }
    

    for(const auto& [artist, albums] : *indexer->get_music_index()){
            for(const auto& [album, tracks] : albums){
                for(const auto& [title, path] : tracks){
                    fs::remove(indexer->get_base_path() / artist / album / path.filename());
                }
                remove(indexer->get_base_path() / artist / album);
            }
            remove(indexer->get_base_path() / artist);
        }

    delete indexer;
}

TEST_CASE("Check permission"){
    fs::create_directory(fs::current_path() / "temp_dir");

    indexer = new Indexer(fs::current_path() / "temp_dir");

    SECTION("Insufficient permission"){
        fs::permissions(fs::current_path() / "temp_dir", fs::perms::owner_read | fs::perms::group_read | fs::perms::others_read,fs::perm_options::replace);
        REQUIRE_THROWS_AS(indexer->check_permission(), fs::filesystem_error);
    }

    SECTION("With permission"){
        fs::permissions(fs::current_path() / "temp_dir", fs::perms::owner_read | fs::perms::owner_write | fs::perms::owner_exec | fs::perms::group_read | fs::perms::group_exec | fs::perms::others_read | fs::perms::others_exec, fs::perm_options::replace);
        REQUIRE_NOTHROW(indexer->check_permission());
    }

    fs::remove(fs::current_path() / "temp_dir");

    delete indexer;
}
