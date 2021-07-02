#include <iostream>
#include <time.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#define SIMD_OPENCV_ENABLE
#include <Simd/SimdLib.hpp>

using namespace std;
using namespace cv;

cv::Mat img,img_resized, left_img,  gray, blurred, edge;


// Adjust the CannyEdge threshold values, recommended ratio is 2:1 or 3:1
int lowerThreshold = 100;
int maxThreshold = 200;
int num_frames = 500;

void Canny(int, void*) {





    cv::GaussianBlur(gray,
        blurred,
        cv::Size(3, 3),
        3);

    cv::Canny(blurred,
        edge,
        lowerThreshold,         // lower threshold
        maxThreshold);           // higher threshold

    cv::imshow("Edge Detection", edge);
}

void simd_imgproc(int, void*){

    typedef Simd::View<Simd::Allocator> View;
    cv::Mat blur;
    View image, out, out2;

    image = left_img; //cv::Mat to Simd view

    Simd::BgrToGray(image, out);
    Simd::GaussianBlur3x3(out, out2);

    blur = out2; //Simd View to cv::Mat

    cv::Canny(blur,
	edge,
	lowerThreshold,
	maxThreshold);

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
        cv::Mat left_img = img(cv::Rect(0, 0, img.cols / 2, img.rows));


	//cvtColor(left_img, gray, COLOR_BGR2GRAY);
	cv::namedWindow("Edge Detection", WINDOW_AUTOSIZE);

        // Canny Edge Detector
	//Canny(0,0);
        simd_imgproc(0,0);
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

