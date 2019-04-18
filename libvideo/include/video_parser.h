#ifndef VIDEO_PARSER_H
#define VIDEO_PARSER_H

#include <string>
#include <vector>
#include <map>
#include <type_traits>

#include <filesystem>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>


#include <yas/serialize.hpp>
#include <yas/std_types.hpp>
#include <yas/file_streams.hpp>

#include "tvshow_info.h"

using namespace lv;

const static char*  unknownPrefix = "unknown";

// Classe pour iterer sur le dataset crée d'une show context
class DatasetExplorer {

};

class VideoTraveler {
protected:
    std::string video_file;
    int current_frame;

public:
    VideoTraveler() {}

    virtual void openNew(std::string video_file) = 0;
    virtual void nextFrame() = 0;    irtual void nextFrame(int count);
    virtual void goToFrame(int count);
    virtual cv::Mat& getFrame();

    int getAtFrame() {
        return this->current_frame;
    }

    const std::string& getVideoFile() {
        return video_file;
    }

};

class CVVideoTraveler : public VideoTraveler {
    cv::Mat                 last_frame;
    cv::VideoCapture        video_capture;


public:
    void openNew(std::string video_file) override;
    void nextFrame () override;
    void nextFrame(int count) override;
    void goToFrame(int count) override;
    cv::Mat& getFrame() override;
};

template<class VT>
class ImageExtractor {
    static_assert(std::is_base_of<VideoTraveler,VT>::value,"VT must inherit from VideoTraveler");

public:

    ShowContext*                        show_context;

    std::vector<cv::Mat>                last_faces;

    cv::Mat                             last_frame;

    std::unique_ptr<VideoTraveler>      video_traveler;

    cv::CascadeClassifier               face_cascade;

    ImageExtractor(ShowContext* ctx) : show_context(ctx) {
        video_traveler = std::make_unique<VT>();
    }

    void SetCascadeClassifier(const fs::path& file) {
        if(!face_cascade.load(file.string())) {
            printf("Impossible de charger le fichier de cascade\n");
        }
    }

    // Ouvre un nouvelle episode pour fetch les données
    void openEpisode(fs::path episode) {
        last_faces.clear();
        video_traveler->openNew(episode.string());
    }

    bool NextFace() {
        last_faces.clear();
        while(last_faces.size() < 1) {
            video_traveler->nextFrame();
            detectFacesImg(video_traveler->getFrame());
        }
        return true;
    }


    void TagAllFace() {
        for(int i = 0;i<last_faces.size();i++) {
            TagFace(unknownPrefix,i);
        }
    }


    void TagFace(std::string name, int index) {
        if(index > last_faces.size()) {
            printf("Index invalide\n");
            return;
        }
        fs::path p = fs::path(show_context->root_folder) / name;
        if(!fs::exists(p)) {
            if(!fs::create_directory(p)) {
                printf("Impossible de créer le dossier %s\n",p.string().c_str());
                return;
            }
        }
        auto fileName = show_context->settings.getFileName(fs::path(video_traveler->getVideoFile()).stem().string(),name,video_traveler->getAtFrame());
        printf("Tag face for %s at frame %d filename %s \n",name.c_str(),video_traveler->getAtFrame(),fileName.c_str());
        p = p / fileName;
        if(!cv::imwrite(p.string(),last_faces[index])) {
            printf("Echec\n");
        }
    }

    // Retourne les visages trouver dans la derniere frame
    std::vector<cv::Mat>& getFacesFrame() {
        return last_faces;
    }

    // Retourne la derniere frame avec les carres de faces
    cv::Mat& getLastFrame() {
        return this->video_traveler->getFrame();
    }

private:

    void detectFacesImg(const cv::Mat& frame) {
        last_frame = frame.clone();
        std::vector<cv::Rect> faces;
        cv::Mat frame_gray;
        cvtColor( frame, frame_gray,cv::COLOR_BGR2GRAY );
        equalizeHist( frame_gray, frame_gray );
        //-- Detect faces
        face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30) );
        for( size_t i = 0; i < faces.size(); i++ )
        {
            cv::Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
            cv::putText(last_frame,std::to_string(i),center,cv::FONT_HERSHEY_COMPLEX,4,{255,255,255},2,cv::LINE_AA);
            rectangle( last_frame, cv::Point(faces[i].x, faces[i].y), cv::Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), cv::Scalar(0,0,255),1,8,0);
            cv::Mat faceROI = frame_gray( faces[i] );//std::vector<cv::Rect> eyes;
            cv::resize(faceROI,faceROI,cv::Size(64,64));
            last_faces.emplace_back(frame(faces[i]));
        }
        printf("Frame %d : %d item detecter\n",video_traveler->getAtFrame(),last_faces.size());
    }
};






#include <boost/python.hpp>

using namespace boost::python;



#endif