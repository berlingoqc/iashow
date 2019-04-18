//
// Created by wq on 19-04-12.
//

#ifndef IASHOW_TVSHOW_INFO_H
#define IASHOW_TVSHOW_INFO_H


#include <filesystem>
#include <string>
#include <map>

#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>

#include <opencv2/core.hpp>

namespace fs = std::filesystem;

using nlohmann::json;

namespace lv {

    template<typename ... Args>
    std::string string_format(const char *format, Args ... args) {
        const size_t size = snprintf(nullptr, 0, format, args ...) + 1;
        std::unique_ptr<char[]> buf(new char[size]);
        snprintf(buf.get(), size, format, args ...);
        return std::string(buf.get(), buf.get() + size - 1);
    }

    struct CharacterInfo {
        int id;
        char *name;
        char shortcut;

        CharacterInfo() : id(-1), name(""), shortcut(0) {}

        CharacterInfo(int i, char *n, char c) : id(i), name(n), shortcut(c) {}

    };

    void to_json(json& j, const CharacterInfo& c) {
        j["id"] = c.id;
        j["name"] = c.name;
        j["shortcut"] = c.shortcut;
    }

    void from_json(const json& j, CharacterInfo& c) {
        j.at("id").get_to(c.id);
        j.at("shortcut").get_to(c.id);
        std::string name = "";
        j.at("name").get_to(name);
        c.name = strdup(name.c_str());
    }


    struct ShowInformation {
        char *show_name;
        std::map<std::string, CharacterInfo> list_character;

    };

    void to_json(json &j, const ShowInformation& s){
        j["show_name"] = s.show_name;
        j["list_character"] = s.list_character;
    }

    void from_json(const json &j,ShowInformation& s) {
        std::string name = "";
        j.at("show_name").get_to(name);
        s.show_name = strdup(name.c_str());
        j.at("list_character").get_to(s.list_character);
    }


    struct DSImageSettings {
        cv::Size img_size;
        std::string img_format;
        std::string name_format;

        std::string getFileName(std::string episode, std::string name, int frame) {
            return string_format("%s_%s_%d.%s", episode.c_str(), name.c_str(), frame, img_format.c_str());
        }

    };

    void to_json(json &j, const DSImageSettings &s) {
        j["img_format"] = s.img_format;
        j["name_format"] = s.name_format;
        j["image_size"] = {{"height", s.img_size.height},
                           {"width",  s.img_size.width}};
    }

    void from_json(const json &j, DSImageSettings &s) {
        j.at("img_format").get_to(s.img_format);
        j.at("name_format").get_to(s.name_format);
        j.at("image_size").at("height").get_to(s.img_size.height);
        j.at("image_size").at("width").get_to(s.img_size.width);
    }

    struct ShowContext {
    public:
        ShowInformation information;
        DSImageSettings settings;

        char *root_folder;

        char *video_source_folder;

        ShowContext() = default;

        ShowContext(fs::path dir, std::string showname) {
            if (!fs::exists(dir) || !fs::is_directory(dir)) {
                printf("Dossier invalide\n");
                return;
            }
            root_folder = strdup(dir.string().c_str());
            information.show_name = strdup(showname.c_str());
        }

        void save() {
            fs::path root = fs::path(root_folder);
            fs::path p = root / information.show_name;
        }

    };


    void to_json(json &j, const ShowContext &c) {
        j["information"] = c.information;
        j["settings"] = c.settings;
        j["root_folder"] = c.root_folder;
        j["video_source_folder"] = c.video_source_folder;
    }

    void from_json(const json &j, ShowContext &c) {
        j.at("information").get_to(c.information);
        j.at("settings").get_to(c.settings);
        std::string root, video = "";
        j.at("root_folder").get_to(root);
        j.at("video_source_folder").get_to(video);
        c.root_folder = strdup(root.c_str());
        c.video_source_folder = strdup(video.c_str());
    }
}


void SaveShowContext(const lv::ShowContext& c) {
    fs::path p = fs::path(c.root_folder);
    p = p / "config.json";
    json j = c;
    std::ofstream file(p.string().c_str());
    if(!file.is_open()) {
       throw "Cant open file lozer";
    }
    file << j;
    file.flush();
    file.close();
}

void LoadShowContext(const std::string& path, lv::ShowContext& ctx) {
    fs::path p = fs::path(path);
    p = p / "config.json";
    std::ifstream file(p.string());
    if(!file.is_open()) {
       throw "Cant open file lozer";
    }
    json j;
    file >> j;
    file.close();
}




#endif //IASHOW_TVSHOW_INFO_H
