#include <iostream>


#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_other.h"

#include "player.h"
#include "imgui_internal.h"

#include "video_parser.h"

RGBGL_Player			player;
GLFWwindow*				window;

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << " : " << description << std::endl;
}

bool init_context()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        std::cerr << "Erreur initialisation glfw" << std::endl;
        return false;
    }

    window = glfwCreateWindow(1600, 900, "ARDRONE GUI", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        std::cerr << "Erreur creation de la fenetre glfw" << std::endl;
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    int v = glewInit();


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    // Setup Style
    ImGui::StyleColorsDark();


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);


    if(!player.setup(1280,720,1920,1080))
    {
        printf("Echec de la configuration du lecteur video\n");
        return false;
    }

    return true;
}

void end_context()
{
    glfwTerminate();
}

ShowContext sc;
ImageExtractor imageExtractor(&sc);

static int v_skip = 1;

void ContextInfoPanel(bool* display) {
    ImGui::Begin("Show context",display);
    ImGui::TextColored({255,255,0,1},"Serie");
    ImGui::Separator();

    ImGui::Text("Nom : %s",sc.information.show_name.c_str());
    ImGui::Text("Repertoire : %s",sc.root_folder.string().c_str());
    ImGui::Text("Repertoire serie : %s",sc.video_source_folder.string().c_str());

    ImGui::Separator();
    ImGui::TextColored({255,255,0,1},"Configuration dataset");
    ImGui::Separator();

    ImGui::Text("Format img : %s",sc.settings.img_format.c_str());
    ImGui::Text("Format nom : %s",sc.settings.name_format.c_str());
    ImGui::Text("Format taille : %d %d",sc.settings.img_size.width,sc.settings.img_size.height);

    ImGui::End();
}

void ImageExtractorPanel(bool* display) {


    ImGui::Begin("ImageExtractor",display);

    ImGui::Text("Episode : %s",imageExtractor.current_file.string().c_str());
    ImGui::Text("Frame : %d",imageExtractor.frame_index);

    ImGui::Separator();

    ImGui::Text("%d face detecter",imageExtractor.last_faces.size());

    ImGui::Separator();

    for(int i = 0;i<imageExtractor.last_faces.size();i++) {
        ImGui::Text("%d",i);
        for(const auto& n : sc.information.list_character) {
            ImGui::SameLine();
            ImGui::PushID(i);
            if(ImGui::Button(n.first.c_str())) {
                imageExtractor.TagFace(n.first,i);
            }
            ImGui::PopID();
        }
    }


    ImGui::Separator();
    if(ImGui::Button("Next")) {
        if(imageExtractor.Next())
            player.setPixels(imageExtractor.getLastFrame());
    }
    ImGui::DragInt("## 1",&v_skip,1,1,1000);
    if(ImGui::Button("Next of")) {
        imageExtractor.Next(v_skip);
        player.setPixels(imageExtractor.getLastFrame());
    }

    if(ImGui::Button("Next face")) {
        if(imageExtractor.NextFace())
            player.setPixels(imageExtractor.getLastFrame());
    }



    ImGui::End();

}

int main() {

    init_context();

    std::string face_cascade_name = "/home/wq/t1/haarcascade_frontalface_alt.xml";


    sc.settings.img_format = "png";
    sc.settings.img_size = cv::Size(64,64);
    sc.settings.name_format = "$episode_$char_$frame.$ext";

    sc.information.show_name = "the_office";
    sc.information.list_character["jim"] = CharacterInfo(1,"jim",'j');
    sc.information.list_character["dwight"] = CharacterInfo(2,"dwight",'d');
    sc.information.list_character["micheal"] = CharacterInfo(3,"micheal", 'm');

    sc.root_folder = "/home/wq/t1";
    sc.video_source_folder = "/var/share/Media/Show/The Office US/";



    imageExtractor.SetCascadeClassifier(face_cascade_name);

    imageExtractor.openEpisode("Season.5/The.Office.S05E04.Crime.Aid.720p.BrRip.x264-[MULVAcoded].mkv");


    static bool show_context_panel = true;
    static bool show_extractor_panel = true;

    static int selected_faces = -1;

    static std::vector<char> last_keys;

    while (!glfwWindowShouldClose(window))
    {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ContextInfoPanel(&show_context_panel);
        ImageExtractorPanel(&show_extractor_panel);


        player.draw();
        ImGuiIO& io = ImGui::GetIO();

        if (io.KeysDown[GLFW_KEY_0]) {
            selected_faces = 0;
        }
        if (io.KeysDown[GLFW_KEY_1]) {
            selected_faces = 1;
        }
        if (io.KeysDown[GLFW_KEY_2]) {
            selected_faces = 2;
        }
        if (io.KeysDown[GLFW_KEY_3]) {

            selected_faces = 3;
        }
        if (io.KeysDown[GLFW_KEY_4])
        {
            selected_faces = 4;
        }
        if (io.KeysDown[GLFW_KEY_5])
        {
            selected_faces = 5;
        }
        if (io.KeysDown[GLFW_KEY_6])
        {
            selected_faces = 6;
        }
        if (io.KeysDown[GLFW_KEY_7])
        {
            selected_faces = 7;
        }
        if (io.KeysDown[GLFW_KEY_8])
        {
            selected_faces = 8;
        }
        if (io.KeysDown[GLFW_KEY_9])
        {
            selected_faces = 9;
        }

        if(io.KeysDown[GLFW_KEY_ENTER]) {
            if(imageExtractor.Next())
                player.setPixels(imageExtractor.getLastFrame());
        }
         if(io.KeysDown[GLFW_KEY_TAB]) {
            if(imageExtractor.NextFace())
                player.setPixels(imageExtractor.getLastFrame());
        }
         if(io.KeysDown[GLFW_KEY_BACKSPACE]) {
             if(imageExtractor.Next(v_skip))
                player.setPixels(imageExtractor.getLastFrame());
        }

        if (io.KeysDown[GLFW_KEY_M])
        {
            if(selected_faces != -1) {
                imageExtractor.TagFace("micheal",selected_faces);
                selected_faces = -1;
            }
        }
        if (io.KeysDown[GLFW_KEY_D])
        {
            if(selected_faces != -1) {
                imageExtractor.TagFace("dwight",selected_faces);
                selected_faces = -1;
            }
        }
        if (io.KeysDown[GLFW_KEY_J])
        {
            if(selected_faces != -1) {
                imageExtractor.TagFace("jim",selected_faces);
                selected_faces = -1;
            }
        }

        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwMakeContextCurrent(window);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
}