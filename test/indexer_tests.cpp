#include <iostream>
#include <string>
#include <unordered_set>

#include <model/Indexer.h>

#include <taglib/fileref.h>

#include <catch2/catch_test_macros.hpp>

Indexer* indexer;

TEST_CASE("Basepath testing") {
    indexer = new Indexer;
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
    indexer = new Indexer;

    std::unordered_set<std::string> artists(3);
    std::unordered_set<std::string> albums(6);
    std::unordered_set<std::string> tracks(18);

    // Make files for testing
    for(int i = 0; i < 3; i++){
        std::string artist_name = "Artist " + std::to_string(i);
        artists.emplace(artist_name);
        for(int j = 0; j < 2; j++){
            std::string album_name = "Album " + std::to_string(j);
            albums.emplace(album_name);
            for(int k = 0; k < 3; k++){
                std::string file_name = "file" + std::to_string(k) + ".mp3";
                std::string track_name = "Track " + std::to_string(k);
                tracks.emplace(tracks);

                std::ofstream file(file_name);
                file.close();

                TagLib::FileRef f(file_name.c_str());

                
                f.tag()->setAlbum(album_name);
                f.tag()->setArtist(artist_name);
                f.tag()->setTitle(track_name);
            }
        }
    }

    delete indexer;
}