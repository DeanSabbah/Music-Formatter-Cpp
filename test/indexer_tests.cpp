#include <iostream>
#include <string>
#include <unordered_set>

#include <model/Indexer.h>

#include <taglib/fileref.h>

#include <catch2/catch_test_macros.hpp>

Indexer* indexer;

TEST_CASE("Basepath testing") {
    indexer = new Indexer(spdlog::level::off);
    fs::path expected_path = fs::current_path();

    REQUIRE(indexer->get_base_path() == expected_path);

    CHECK_THROWS_AS(indexer->set_base_path("Eat/My/Ass"), fs::filesystem_error);
    
    fs::path new_path = fs::path(fs::current_path() / "test");
    
    fs::create_directories(new_path);
    indexer->set_base_path(new_path);
    
    REQUIRE(indexer->get_base_path() == new_path);

    fs::remove(new_path);

    delete indexer;
}

TEST_CASE("Indexing files") {
    indexer = new Indexer(spdlog::level::off);

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

    for(auto file : track_paths){
        fs::remove(file);
    }

    delete indexer;
}
