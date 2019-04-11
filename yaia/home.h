//
// Created by wq on 19-04-10.
//

#ifndef IASHOW_HOME_H
#define IASHOW_HOME_H


#include "imgui.h"
#include "imgui_abstractwindow.h"
#include "imgui_fs.h"
#include "app.h"


enum EOptionOpening { NewProject, OpenProject};

class HomeWindow : public bsl::ui::AbstractWindow {

public:
    HomeWindow() {

    }

    EOptionOpening optionOpening;
    std::string newProjectName;
    std::string newProjectLocation;

    std::string openProjectConfig;


    void Draw(bool* over,bool* p_open) override {

        static bool wait_file = false;
        static bool wait_file_new = false;
        static char	buffer_name[100];
        static char buffer_path[100];
        static bsl::ui::FileExplorer fx;

        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPosCenter();
        if (ImGui::Begin("Acceuil", p_open)) {

            ImGui::TextColored({255, 255, 0, 1}, "IAShow Intelligence Artificelle for tv Show");
            ImGui::Separator();

            ImGui::InputText("Project Name", buffer_name, 100);
            ImGui::InputText("Folder", buffer_path, 100);
            ImGui::SameLine();
            if (ImGui::Button("Browse")) {
                fx.get_folder = true;
                fx.Start();
                wait_file_new = true;
            }
            if (ImGui::Button("Create new project")) {
                    this->newProjectLocation = buffer_path;
                    this->newProjectName = buffer_name;
                    this->optionOpening = NewProject;
                    *over = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Load Existing project")) {
                fx.get_folder = false;
                fx.Start();
                wait_file = true;
            }

            if (wait_file_new) {
                fx.Draw();
                if (fx.is_close) {
                    wait_file_new = false;
                    if (!fx.selected_current.empty())
                        strcpy(buffer_path, fx.selected_current.string().c_str());
                }
            }

            if (wait_file) {
                fx.Draw();
                if (fx.is_close) {
                    wait_file = false;
                    if (!fx.selected_current.empty()) {
                        this->openProjectConfig = fx.selected_current.string();
                        this->optionOpening = OpenProject;
                        *over = true;
                    }
                }
            }
            ImGui::End();
        }
    }
};
#endif //IASHOW_HOME_H
