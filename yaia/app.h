//
// Created by wq on 19-04-10.
//

#ifndef IASHOW_APP_H
#define IASHOW_APP_H

#include "imgui.h"
#include "imgui_baseapp.h"
#include "imgui_abstractwindow.h"
#include "home.h"
#include "new_project.h"



enum IashowState { State_HomeWindow, State_VideoExtractor, State_NewProject};




class IashowApp : public bsl::ui::BaseApp {

    bsl::ui::AbstractWindow* currentWindow = nullptr;
    IashowState  state = State_HomeWindow;

public:
    IashowApp() {
        this->applicationName = "Iashow";
        this->backgroundColor = glm::vec4(0.2,0.6,0.2,1.0);

        currentWindow = new HomeWindow();
    }

    void TransitionHomeWindow() {
        if(currentWindow!=nullptr){
            HomeWindow* homeWindow = static_cast<HomeWindow*>(currentWindow);
            switch (homeWindow->optionOpening) {
                case NewProject:
                    std::cout << "NewProject" << std::endl;
                    state = State_NewProject;
                    //NewProjectWindow* nextWindow = new NewProjectWindow(fs::path(homeWindow->newProjectLocation),homeWindow->newProjectName);
                    currentWindow = new NewProjectWindow(fs::path(homeWindow->newProjectLocation),homeWindow->newProjectName);
                    break;
                case OpenProject:
                    std::cout << "Opening" << std::endl;
                    state = State_VideoExtractor;
                    break;
            }
        }

    }

    void TransitionNewProject() {

    }

    void TransitionVideoExtractor() {

    }

    void StateTransition() {
        switch (state) {
            case State_HomeWindow:
                TransitionHomeWindow();
                break;
            case State_NewProject:

                break;
            case State_VideoExtractor:

                break;
        }

    }



protected:


    void mainLoop() override {
        static bool isCurrentWindowOver = false;
        static bool showCurrentWindow = true;
        if(currentWindow != nullptr) {
            currentWindow->Draw(&isCurrentWindowOver,&showCurrentWindow);
            if(isCurrentWindowOver) {
                StateTransition();
                isCurrentWindowOver = false;
            }
        }

    }

};

#endif //IASHOW_APP_H
