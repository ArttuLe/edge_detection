#include <iostream>
#include <ctime>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/dnn/dnn.hpp>

using namespace std;
using namespace cv;

Mat img, gray, blurred, edge;


// Adjust the CannyEdge threshold values
int lowerThreshold = 100;
int maxThreshold = 200;


void CannyThreshold(int, void*) {

    GaussianBlur(gray,  //Image preprocessing
        blurred,
        cv::Size(3, 3), 
        3);              

    Canny(blurred,
        edge,
        lowerThreshold,         // lower threshold
        maxThreshold);           // higher threshold

    imshow("Edge Detection", edge);
}



int main() {


    VideoCapture cap;

    int deviceID = 0;
    int  apiID = cv::CAP_ANY;

    cap.open(deviceID, apiID);

    clock_t start_ticks, end_ticks;

    if (!cap.isOpened())
    {
        cout << "Could not open camera" << endl;
        return -1;
    }
     
	while(1){
		start_ticks = clock();

        cap.read(img);
	    cvtColor(img, gray, COLOR_BGR2GRAY);

	    cv::namedWindow("Edge Detection", WINDOW_AUTOSIZE);


        // Canny Edge Detector
	    CannyThreshold(0,0);
        
        end_ticks = clock();
        cout << "Time: " << 1000.0 * (end_ticks-start_ticks)/CLOCKS_PER_SEC << " ms\n";


        imshow("Edge Detection", edge);

        if (waitKey(5) >= 0)
             break;
    }
    

  return 0;
}

