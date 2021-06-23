#include <iostream>
#include <time.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>


using namespace std;
using namespace cv;

Mat img,img_resized, left_img, gray, blurred, edge;


// Adjust the CannyEdge threshold values, recommended ratio is 2:1 or 3:1
int lowerThreshold = 100;
int maxThreshold = 200;
int num_frames = 500;

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
    int  apiID = cv::CAP_V4L2; //Videocapturing via libv4l
    cv::setUseOptimized(1);

    cap.open(deviceID, apiID);

    //Set image size 720p 2560x720, WVGA 1344x376
    cap.set(CAP_PROP_FRAME_WIDTH, 2560);
    cap.set(CAP_PROP_FRAME_HEIGHT, 720);

    if( !cap.isOpened() )
    {
        cout << "Cannot open camera" <<endl;

        return -1;
    }

    if(cv::useOptimized()){

	cout<< "Optimized code enabled..." << endl;

    }else

        cout<< "Optimized code disabled..." << endl;

    int counter = 0;


    time_t start,end;


    time(&start);

    while(counter < num_frames){

        double start = (double)getTickCount();

	cap >> img;

        //Zed has 2 cameras, extract them as separate pictures.
        left_img = img(cv::Rect(0, 0, img.cols / 2, img.rows));
        // right_img = img(cv::Rect(img.cols / 2, 0, img.cols / 2, img.rows));
	//cv::resize(left_img, img_resized, Size(), 0.25, 0.25, 3);

	cvtColor(left_img, gray, COLOR_BGR2GRAY);
	cv::namedWindow("Edge Detection", WINDOW_AUTOSIZE);

        // Canny Edge Detector
	Canny(0,0);
	counter++;


        double end = (double)getTickCount();

        cout << "Fps: "<<(getTickFrequency()/ (end-start))<<endl;

        if (waitKey(1) >= 0)
             break;
    }
    time(&end);

    cout << "Average fps over "<< num_frames << " frames: " << (num_frames / (end-start))<<endl;




  return 0;
}

