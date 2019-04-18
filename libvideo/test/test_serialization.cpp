
#define CATCH_CONFIG_MAIN

#include <string>
#include <catch2/catch.hpp>
#include <iostream>

#include "video_parser.h"

using namespace lv;

auto context = ShowContext();
auto contextLoaded = ShowContext();

std::string location = "/home/wq/theoffieia";

const static fs::path baseFolder = "/var/share/Media/Show/The Office US";
void loadData() {
    auto michel = CharacterInfo();
    michel.shortcut = 'c';
    michel.name = strdup("Michel Scott");

    auto showContext = ShowInformation();
    showContext.show_name = strdup("The office (US)");
    showContext.list_character.insert(std::pair("dsad",michel));

    auto imageSettings = DSImageSettings();
    imageSettings.img_size = { 64,64 };
    imageSettings.img_format = "png";
    imageSettings.name_format = "bw";

    context.information = showContext;
    context.settings = imageSettings;
    context.root_folder = strdup(location.c_str());
    context.video_source_folder = strdup("/var/share/Media/Show");


    json j = context;
    std::cout << j.dump();
    //json j2 = contextLoaded;
    //REQUIRE(j.dump() == j2.dump());
}

TEST_CASE("Testing saving of the file", "[saveconfig]") {
    REQUIRE_NOTHROW([&](){
        loadData();
        SaveShowContext(context);
    });
}

TEST_CASE("Testing loading of the file", "[loadconfig]") {
     REQUIRE_NOTHROW([&](){
        loadData();
        LoadShowContext(location,contextLoaded);
    });
}


TEST_CASE("Testing open cv video traveler with extractor","[cvvideotraveler]") {
    auto imageExtractor = new ImageExtractor<CVVideoTraveler>(&context);
    imageExtractor->SetCascadeClassifier("/home/wq/t1/haarcascade_frontalface_alt.xml");
    imageExtractor->openEpisode(baseFolder / "Season.4/The.Office.S04E05E06.Launch.Party.WEB-DL.x264-[MULVAcoded].mkv");
    for(int i = 0;i < 10;i++) {
        if(imageExtractor->NextFace()) {
            imageExtractor->TagAllFace();
        }
    }
}