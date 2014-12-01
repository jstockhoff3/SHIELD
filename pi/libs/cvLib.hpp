/*
 * Although the majority of this code is original, 
 * some of the concepts and functions were taken from code with the following copywrite(s)
 *
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace cv;
using namespace std;

vector<Rect> detectPeople(Mat);
void detectFaces(Mat frame, std::vector<cv::Rect_<int> > *theFaces,CascadeClassifier theCascade);
string recognizeFace(Mat *faceFrame, string csv_path, int image_width, int image_height, Ptr<FaceRecognizer> model, double *predictionConfidence);
static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels);
string getSubjectName(int thePrediction, double theConfidence, string csv_path);
Ptr<FaceRecognizer> initializeFaceRecognizer();
CascadeClassifier initializeFrontalFaceCascade();

/**
 * People detection using HoG.
 */

vector<Rect> detectPeople(Mat frame) {
  HOGDescriptor hog;
  vector<Rect> found, filtered;
  size_t i, j;

  hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
  hog.detectMultiScale(
    frame, 
    found, 
    0, 
    Size(8,8), 
    Size(32,32), 
    1.05, 
    2
  );

  for(i = 0; i < found.size(); i++){
    Rect r = found[i];
    
    for(j = 0; j < found.size(); j++) 
      if(j != i && (r & found[j]) == r)
        break;
    
    if(j == found.size())
      filtered.push_back(r);
  }

  for(i = 0; i < filtered.size(); i++){
    Rect r = filtered[i];
    
    r.x += cvRound(r.width*0.1);
    r.width = cvRound(r.width*0.8);
    r.y += cvRound(r.height*0.07);
    r.height = cvRound(r.height*0.8);
    
    rectangle(frame, r.tl(), r.br(), Scalar(0,255,0), 1);        
  }

  cout << "--------------------------------------------" << endl;
  cout << "function: detectPeople" << endl;
  cout << "--------------------------------------------" << endl;

  return filtered;
}

/**
 * Detect faces.
 */

void detectFaces(Mat frame, vector<Rect> *theFaces, CascadeClassifier theCascade){
    
    Mat frame_gray;

    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    
    theCascade.detectMultiScale(frame_gray, *theFaces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
}

/**
 * Read a csv file.
 */

static void read_csv(const string& filename, vector<Mat> &images, vector<int> &labels) {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;

    cout << "Reading in images for comparison... ";
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, ';');
        getline(liness, classlabel);
        cout << "liness: " << liness << "classlabel: " << classlabel << "path: " << path << endl;
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
    cout << "Done!" << endl;
}

/**
 * Recognize a single face.
 */
string recognizeFace(Mat *faceFrame, string csv_path, int image_width, int image_height, Ptr<FaceRecognizer> theModel, double *predictionConfidence) {
  Mat gray;
  cvtColor(*faceFrame, gray, CV_BGR2GRAY);

  // Crop the face from the image. So simple with OpenCV C++:

  Mat face_resized;

  cv::resize(gray, face_resized, Size(image_width, image_height), 1.0, 1.0, INTER_CUBIC);

  
  //imshow("Face",face);
  // Now perform the prediction:
  int predictionID;
  theModel->predict(face_resized,predictionID,*predictionConfidence);


  // And now put it into the image:

  string subj_name = getSubjectName(predictionID,*predictionConfidence,csv_path);
  //putText(*mainFrame, format("%s %f",subj_name.c_str(),predictionConfidence), Point(x, y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);

  //cout << "name: " << subj_name << ", confidence: " << (double)predictionConfidence << endl;
  cout << "Prediction ID = " << predictionID << endl; 
  return format("%s",subj_name.c_str());
}


/**
 * Return the subject name for a given
 * prediction.
 */

string getSubjectName(int thePrediction, double theConfidence, string csvPath) {
  double confidenceThresh = 800;

  if(theConfidence > confidenceThresh){
       return "unknown";
  }

  string predictionString = format("%d",thePrediction);
  ifstream file(csvPath.c_str());
  string line;
  string base_path = "libs/facerec/data/at/";
  string subj_name;
  bool done = 0;
  
  while(file.good()){
    getline(file, line, ';'); // read a string until ';'
    if(line.find(predictionString) != std::string::npos){
      //this line contains the info we are looking for (the name of the matched subject) 
      return line.substr(base_path.length()+predictionString.length()+1,line.length()-base_path.length()-predictionString.length()-7);
    }
  }

  return "something went wrong";
}

Ptr<FaceRecognizer> initializeFaceRecognizer() {

    Ptr<FaceRecognizer> recModel;
    int imWidth, imHeight;
    string csvPath = "libs/faceLearn.csv";
    vector<Mat> images;
    vector<int> labels;

    try {
      read_csv(csvPath, images, labels);
    } catch (cv::Exception& e) {
      cerr << "Error opening file \"" << csvPath << "\". Reason: " << e.msg << endl;
      exit(-1);
    }

    if(images.size()){
      imWidth = images[0].cols;
      imHeight = images[0].rows;

      if(imWidth != 92 || imHeight != 112){
        cout << "Training images are the wrong size." << endl;
        cout << "(imHeight,imWidth) = " << "(" << imHeight << "," << imWidth << ")" << endl;
        exit(-1);
      }
      recModel = createFisherFaceRecognizer();
      recModel->train(images, labels);
    }else{
        cout << "No images with which to train the recognizer." << endl;
        exit(-1);
    }
 
    cout << "Recognizer model initiated successfully." << endl;
    return recModel;
}

CascadeClassifier initializeFrontalFaceCascade(){
    CascadeClassifier frontalFaceCascade;
    string faceCascadeName = "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml"; 
    frontalFaceCascade.load(faceCascadeName);
    return frontalFaceCascade;
}

