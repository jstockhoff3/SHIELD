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
        cout << "\t </path/for/image> -- The path at which to save the image." << endl;
        exit(1);
    }else{

        string image_path = string(argv[1]);

        VideoCapture cap(-1); // open the default camera
        if(!cap.isOpened()){  // check if we succeeded
            cout << "Camera Initialization Failed." << endl;
            return -1;
        }

        cout << "Capture Initialized Successfully." << endl;

        Mat frame;

        int i;
        int warmupIterations = 5;
        cout << "Camera Warming Up";
        for(i=0;i<warmupIterations;i++)
        {
            cout << ".";
            cap >> frame;
            cvWaitKey(100);
        }

        cout << endl << "Warmup Complete! Capturing Image Now." << endl;

        cap >> frame; // get a new frame from camera
        imwrite(image_path, frame); //save frame to image_path
        uploadImage((char *)image_path.c_str()); //upload image to server
        
        //the camera will be deinitialized automatically in VideoCapture destructor
    }
    return 0;
}