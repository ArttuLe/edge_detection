#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/cudafilters.hpp>

using namespace std;
using namespace cv;
using namespace cuda;

Mat img, right_img, left_img,output;
GpuMat gpu_img, gray, blurred, edge;


// Adjust the CannyEdge threshold values, recommended ratio is 2:1 or 3:1
int lowerThreshold = 50;
int maxThreshold = 150;


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

        //download the image into gpu memory
        gpu_img.upload(left_img);

	    cuda::cvtColor(gpu_img, gray, cv::COLOR_BGR2GRAY);

	    cv::namedWindow("Edge Detection", WINDOW_AUTOSIZE);

        Ptr<cuda::Filter> gaussianFilter = cuda::createGaussianFilter(gray.type(),
        blurred.type(),
        cv::Size(3, 3),
        0);

        //Add gaussian blur
        gaussianFilter->apply(gray,blurred);

        Ptr<cuda::CannyEdgeDetector> cannyFilter = cuda::createCannyEdgeDetector(lowerThreshold, maxThreshold);
        
        //Apply edge detection
        cannyFilter->detect(blurred, edge);

        //download result from gpu
        edge.download(output);
        
        int fps = getTickFrequency() / ((double)getTickCount() - t);
        cout << "FPS: " << fps << "\n";

        imshow("Edge Detection", output);

        if (waitKey(5) >= 0)
             break;
    }
    

  return 0;
}

