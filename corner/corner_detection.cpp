#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>

using namespace std;
using namespace cv;

Mat img, frame, gray, dst_norm, dst_scaled, corners;


// Adjust the threshold values, recommended ratio is 2:1 or 3:1
int thresh = 100;
int max_threshold = 200;


void Harris(int, void*) {
    int blockSize = 3;
    int apertureSize = 3;
    double k = 0.04;

    //Detect corners
    cornerHarris(gray, corners, blockSize, apertureSize, k);

    //normalize the output from the cornerdetection
    normalize(corners, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_scaled);

    for( int i = 0; i < dst_norm.rows ; i++ )
    {
        for( int j = 0; j < dst_norm.cols; j++ )
        {
            if( (int) dst_norm.at<float>(i,j) > thresh )
            {
                circle( dst_scaled, Point(j,i), 5,  Scalar(0), 2, 8, 0 );
            }
        }
    }




    imshow("Corner Detection", corners);
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

        //Zed has 2 cameras, extract only one of them
        frame = img(cv::Rect(0, 0, img.cols / 2, img.rows));
        


	    cvtColor(frame, gray, COLOR_BGR2GRAY);
	    cv::namedWindow("Corner Detection", WINDOW_AUTOSIZE);

        // Canny Edge Detector
	    Harris(0,0);
        imshow("Corner Detection", dst_scaled);

        int fps = getTickFrequency() / ((double)getTickCount() - t);
        cout << "fps: " << fps << "\n";


        if (waitKey(5) >= 0)
             break;
    }
    

  return 0;
}
