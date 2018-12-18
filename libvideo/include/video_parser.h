#ifndef VIDEO_PARSER_H
#define VIDEO_PARSER_H

#include <string>
#include <vector>
#include <map>

#include <filesystem>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>


#include <yas/serialize.hpp>
#include <yas/std_types.hpp>
#include <yas/file_streams.hpp>


namespace fs = std::filesystem;

constexpr std::size_t  flags_yas_bf = yas::file | yas::binary;


struct CharacterInfo {
    int             id;
    std::string     name;
    char            shortcut;

    CharacterInfo() : id(-1),name(""),shortcut(0) {}
    CharacterInfo(int i, std::string n,char c) : id(i), name(std::move(n)), shortcut(c) {}

    template<typename Ar>
    void serialize(Ar& ar) {
        ar & YAS_OBJECT(nullptr,id,name,shortcut);
    }
};


struct ShowInformation {
    std::string                                 show_name;
    std::map<std::string,CharacterInfo>         list_character;

    template<typename Ar>
    void serialize(Ar& ar) {
        ar & YAS_OBJECT(nullptr,show_name,list_character);
    }
};


struct DSImageSettings {
    cv::Size    img_size;
    std::string img_format;
    std::string name_format;

    template<typename Ar>
    void serialize(Ar& ar) {
        ar & YAS_OBJECT(nullptr,img_size,img_format);
    }
};


class ShowContext {
public:
    ShowInformation     information;
    DSImageSettings     settings;

    fs::path            root_folder;

    fs::path            video_source_folder;



    void createNew(fs::path dir,std::string showname) {
        if(!fs::exists(dir) || !fs::is_directory(dir)) {
            printf("Dossier invalide\n");
            return;
        }
        root_folder = dir;
        information.show_name = showname;

        save();
    }

    void load(const fs::path& configfile) {
        //yas::file_istream ifs(configfile.string().c_str());
        //yas::load<flags_yas_bf>(ifs,information,settings);

    }

    void save() {
        //fs::path p = root_folder / information.show_name;
        //yas::file_ostream of(p.string().c_str(), yas::file_trunc);
        //yas::save<flags_yas_bf>(of,information,settings);
        //of.flush();
    }

};


class ImageExtractor {

public:

    ShowContext*            show_context;

    fs::path                cc_file;

    fs::path                current_file;

    std::vector<cv::Mat>    last_faces;

    cv::Mat                 last_frame;

    int                     frame_index;

    cv::VideoCapture        video_capture;

    cv::CascadeClassifier   face_cascade;

    ImageExtractor(ShowContext* ctx) : show_context(ctx) , current_file(""), frame_index(-1) {

    }

    void SetCascadeClassifier(const fs::path& file) {
        if(!face_cascade.load(file.string())) {
            printf("Impossible de charger le fichier de cascade\n");
        }
    }

    cv::Size getCurrentVideoSize() {
        return last_frame.size();
    }

    // Ouvre un nouvelle episode pour fetch les données
    void openEpisode(fs::path episode) {
        current_file = episode;
        episode = show_context->video_source_folder / episode;
        video_capture = cv::VideoCapture(episode.string().c_str());
        frame_index = -1;
        last_faces.clear();
        if(!video_capture.isOpened()) {
            printf("Impossible ouvrir video\n");
            current_file = "";
            return;
        }
        Next();
    }


    bool Next() {
        if(video_capture.isOpened()) {
            cv::Mat m;
            bool b = video_capture.read(m);
            if(!b) return false;
            if(m.empty()) return false;
            // Tout est chill ici
            frame_index++;
            last_faces.clear();
            detectFacesImg(m);

            return true;
        }
        return false;
    }

    bool Next(int frame_skip) {
        for(int i = 0;i<frame_skip-1;i++) {
            video_capture.read(last_frame);
            frame_index++;
        }
        return Next();
    }

    bool NextFace() {
        last_faces.clear();
        while(last_faces.size() < 1) {
            if(!Next())
                return false;
        }
        return true;
    }

    std::vector<cv::Mat>& getFacesFrame() {
        return last_faces;
    }

    cv::Mat& getLastFrame() {
        return last_frame;
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
            cv::resize(faceROI,faceROI,show_context->settings.img_size);
            last_faces.emplace_back(frame(faces[i]));
        }

        printf("Frame %d : %d item detecter\n",frame_index,last_faces.size());
    }

};



#endif