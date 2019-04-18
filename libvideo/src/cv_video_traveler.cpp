//
// Created by wq on 19-04-14.
//
#include "video_parser.h"


void CVVideoTraveler::openNew(std::string video_file) {
    this->video_capture = cv::VideoCapture(video_file.c_str());
    this->current_frame = -1;
    if(!video_capture.isOpened()) {
        throw "Impossible ouvrir video\n";
    }
    this->video_file = video_file;
    nextFrame();
}

cv::Mat& CVVideoTraveler::getFrame() {
    return last_frame;

}

void CVVideoTraveler::goToFrame(int n) {
    if(n < current_frame) {
        throw "Can't go back with CVVideoTraveler";
    }
    auto nbrToSkip = n - current_frame;
    nextFrame(nbrToSkip);
}

void CVVideoTraveler::nextFrame() {
    if(video_capture.isOpened()) {
        bool b = video_capture.read(last_frame);
        if(!b || last_frame.empty()) {
            throw "fail next frame";
        };
        // Tout est chill ici
        current_frame++;
    } else {
        throw "video capture not opened";
    }
}

void CVVideoTraveler::nextFrame(int nbr) {
    for(int i = 0;i<nbr-1;i++) {
        video_capture.read(last_frame);
        current_frame++;
    }
    nextFrame();

}
