//
// Created by wq on 19-04-10.
//

#ifndef IASHOW_NEW_PROJECT_H
#define IASHOW_NEW_PROJECT_H


#include <header.h>
#include "imgui.h"
#include "imgui_abstractwindow.h"

#include "video_parser.h"

void FormShowInformation(ShowInformation* showInfo) {
    ImGui::TextColored({255,255,0,1}, "Show information ");
    ImGui::Separator();
    ImGui::InputText("Show name ",showInfo->show_name);
}

class NewProjectWindow : public bsl::ui::AbstractWindow {


    fs::path folder;
    std::string name;


public:

    NewProjectWindow(fs::path folder, std::string name) {
        this->folder = folder;
        this->name = name;
    }

    void Draw(bool *over,bool *p_open) override {
        ImGui::SetNextWindowSize(ImVec2(600,800), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPosCenter();
        ImGui::Begin("NewProject",nullptr);

        ImGui::TextColored({255,255,0,1}, "Configure your project");
        ImGui::Separator();
        ImGui::Text(("Folder "+ folder.string()).c_str());
        ImGui::Text(("Name   "+name).c_str());
        ImGui::Separator();



        ImGui::End();

    }
};

#endif //IASHOW_NEW_PROJECT_H
