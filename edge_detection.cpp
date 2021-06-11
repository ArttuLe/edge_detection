#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/core/utility.hpp>

using namespace std;
using namespace cv;

Mat img, right_img, left_img, gray, blurred, edge;


// Adjust the CannyEdge threshold values, recommended ratio is 2:1 or 3:1
int lowerThreshold = 50;
int maxThreshold = 150;


void Canny(int, void*) {

    GaussianBlur(gray,
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

    //Set image size
    cap.set(CAP_PROP_FRAME_WIDTH, 2560);
    cap.set(CAP_PROP_FRAME_HEIGHT, 720);


    if (!cap.isOpened())
    {
        cout << "Could not open camera" << endl;
        return -1;
    }

	while(1){

        double t = (double)getTickCount();
        cap.read(img);

        //Zed has 2 cameras, extract them as separate pictures.
        left_img = img(cv::Rect(0, 0, img.cols / 2, img.rows));
        right_img = img(cv::Rect(img.cols / 2, 0, img.cols / 2, img.rows));


	    cvtColor(left_img, gray, COLOR_BGR2GRAY);
	    cv::namedWindow("Edge Detection", WINDOW_AUTOSIZE);

        // Canny Edge Detector
	    Canny(0,0);

        int fps = getTickFrequency() / ((double)getTickCount() - t);
        cout << "FPS: " << fps << "\n";


        if (waitKey(5) >= 0)
             break;
    }
    

  return 0;
}

