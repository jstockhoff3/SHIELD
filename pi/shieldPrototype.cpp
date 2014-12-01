#include "opencv2/opencv.hpp"
#include "libs/shieldHttpActionsLib.hpp"
#include "libs/cvLib.hpp"
#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;
using namespace cv;



int main(int argc, const char *argv[])
{

    if (argc != 2) {
        cout << "usage: " << argv[0] << " </path/for/image.png> " << endl;
        cout << "\t </path/for/image> -- The path at which to save the image. (both .png and .jpg are supported)" << endl;
        exit(1);
    }else{

        string image_path = string(argv[1]);

        VideoCapture cap(-1); // open the default camera
        if(!cap.isOpened()){  // check if we succeeded
            cout << "Camera Initialization Failed." << endl;
            return -1;
        }

        cout << "Capture Initialized Successfully." << endl;
        cout << "Initializing facerecognizer" << endl;

        bool enableFaceRecognition = 1;
        Ptr<FaceRecognizer> faceRecModel = initializeFaceRecognizer();
        CascadeClassifier frontalFaceCascade = initializeFrontalFaceCascade();
        Mat frame;
        vector<Rect>faces;
        vector<Rect>oldFaces;
        vector<String>nameVector;
        vector<double>confVector;
        string name;
        string csvPath = "libs/faceLearn.csv";
        double theConfidence;
        int timesUnknown = 0;

        bool stop = 0;
        while(!stop)
        {   
            cout << "capturing frame ";
            cap >> frame;
            cout << "done" << endl;

            cout << "detecting faces ";
            detectFaces(frame, &faces, frontalFaceCascade);
            cout << " done" << endl;

            if(faces.size() && faces.size() == oldFaces.size()){
                cout << "faces detected" << endl;
                Mat faceFrame;
                for(size_t i = 0; i < faces.size(); i++){
                    Rect face = faces[i];
                    faceFrame = frame(face);
                    name = recognizeFace(&faceFrame, csvPath, 92, 112, faceRecModel, &theConfidence);
                    nameVector.insert(nameVector.begin()+i,name);
                    confVector.insert(confVector.begin()+i,theConfidence);

                    cout << "name: " << name << "   conf: " << theConfidence << endl;
                    Point center(faces[i].x + faces[i].width * 0.5, faces[i].y + faces[i].height * 0.5);
                    ellipse(frame, center, Size(faces[i].width * 0.5, faces[i].height * 0.5), 0, 0, 360, Scalar(0, 255, 0), 2, 8, 0);
                    putText(frame, format("%s %.2f",nameVector[i].c_str(),confVector[i]), Point(face.x, face.y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
                }

                cout << "stopping loop" << endl;
                stop = 1;
            }else{
                cout << "faces.size() = " << faces.size() << endl;
            }

            oldFaces = faces;
            //cvWaitKey(100);
        }  
        
        imwrite(image_path, frame); //save frame to image_path
        uploadImage((char *)image_path.c_str()); //upload image to server
        
        //the camera will be deinitialized automatically in VideoCapture destructor
    }
    return 0;
}