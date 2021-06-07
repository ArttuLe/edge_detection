#include <iostream>
#include <vector>
#include <unistd.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/dnn/dnn.hpp>

using namespace std;
using namespace cv;

Mat img, gray, blurred, edge;


// Canny Edge Detection Variables
int lowerThreshold = 100;
int max_lowThreshold = 200;


void CannyThreshold(int, void*) {

    GaussianBlur(gray,
        blurred,
        cv::Size(3, 3),  // smoothing window width and height in pixels
        3);              // how much the image will be blurred

    Canny(blurred,
        edge,
        lowerThreshold, // lower threshold
        50);           // higher threshold

    imshow("Edge Detection", edge);
}



int main() {


    VideoCapture cap;

    int deviceID = 0;
    int  apiID = cv::CAP_ANY;

    cap.open(deviceID, apiID);


    if (!cap.isOpened())
    {
        cout << "Could not open camera" << endl;
        return -1;
    }
     
	while(1){
		
        cap.read(img);
	    cvtColor(img, gray, COLOR_BGR2GRAY);

	    cv::namedWindow("Edge Detection", WINDOW_AUTOSIZE);


        // Canny Edge Detector
	    //createTrackbar("Min Threshold:", "Edge Detection", &lowerThreshold, max_lowThreshold, CannyThreshold);
	    CannyThreshold(0,0);


        imshow("Edge Detection", edge);
        if (waitKey(5) >= 0)
             break;
    }
    

  return 0;
}

