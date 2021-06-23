#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/core/utility.hpp>

using namespace std;
using namespace cv;

int main(){


    VideoCapture cam;

    int deviceID = 0;
    int apiID = cv::CAP_ANY;

    cam.open(deviceID, apiID);

    cam.set(CAP_PROP_FRAME_WIDTH, 2560);
    cam.set(CAP_PROP_FRAME_HEIGHT, 720);


    if(!cam.isOpened()){

     cout << "Error with opening the camera..." << endl;
     return -1;

    }


    //Capture the starting point image
    Mat img, frame1, prev;
    cam.read(img);
    frame1 = img(cv::Rect(0, 0, img.cols / 2, img.rows));
    cvtColor(frame1, prev, COLOR_BGR2GRAY);


    while(1){

	Mat frame2, img2, next;

	double t = (double)getTickCount();
	cam.read(img2);
	frame2 = img2(cv::Rect(0, 0, img2.cols / 2, img2.rows));
	cvtColor(frame2, next, COLOR_BGR2GRAY);

	//Calculate the optical flow with Farneback's algorithm
	Mat flow(prev.size(), CV_32FC2);
	calcOpticalFlowFarneback(prev, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

	//Visualize the optical flow
	Mat flow_parts[2];
	split(flow, flow_parts);
	Mat magnitude, angle, magn_norm;
	cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
	normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
	angle *= ((1.f / 360.f) * (180.f / 255.f));

	//build hsv image
	Mat _hsv[3], hsv, hsv8, bgr;
	_hsv[0] = angle;
	_hsv[1] = Mat::ones(angle.size(), CV_32F);
	_hsv[2] = magn_norm;
	merge(_hsv, 3, hsv);
	hsv.convertTo(hsv8, CV_8U, 255.0);
	cvtColor(hsv8, bgr, COLOR_HSV2BGR);


	imshow("frame2", bgr);





	int fps = getTickFrequency() / ((double)getTickCount() - t);
	cout << "FPS: " << fps << endl;


	if(waitKey(5) >= 0){
	    break;
	}

	}

	return 0;

    }
