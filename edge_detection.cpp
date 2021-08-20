#include <iostream>
#include <time.h>
#include <thread>
#include <stack>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>


using namespace std;
using namespace std::chrono;



// Adjust the CannyEdge threshold values, recommended ratio is 2:1 or 3:1
int lowerThreshold = 100;
int maxThreshold = 200;
int num_frames = 200;


void image_processing(cv::Mat &input_mat, cv::Mat &output) {

    cv::Mat gray, blurred;

    cvtColor(input_mat, gray, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(gray,
        blurred,
        cv::Size(3, 3),
        3);

    cv::Canny(blurred,
        output,
        lowerThreshold,       
        maxThreshold);           

    cv::imshow("Edge Detection", output);
}


class Parallel_process : public cv::ParallelLoopBody
    {

    private:
        cv::Mat img;
        cv::Mat& retVal;
        int diff;

    public:
        Parallel_process(cv::Mat inputImage, cv::Mat& outImage, int diffVal)
            : img(inputImage), retVal(outImage), diff(diffVal){}

        virtual void operator()(const cv::Range& range) const
        {
            for(int i = range.start; i < range.end; i++)
            {
                cv::Mat in(img, cv::Rect(0, (img.rows/diff)*i, img.cols, img.rows/diff));
                cv::Mat blurred(img, cv::Rect(0, (img.rows/diff)*i, img.cols, img.rows/diff));
                cv::Mat gray(img, cv::Rect(0, (img.rows/diff)*i, img.cols, img.rows/diff));

                cv::Mat out(retVal, cv::Rect(0, (retVal.rows/diff)*i, retVal.cols, retVal.rows/diff));

                cvtColor(in, gray, cv::COLOR_BGR2GRAY);
                cv::GaussianBlur(gray,blurred,cv::Size(3,3),3);

                cv::Canny(blurred,out, lowerThreshold, maxThreshold);


            }
        }
    };



int main() {

    cv::VideoCapture cap;
    std::stack<cv::Mat> stack; //Allocate memory beforehand to avoid issues with dynamic allocation?
    cv::Mat img, left_img, gray, out;

    int deviceID = 0;
    int  apiID = cv::CAP_V4L2;    //cv::CAP_V4L2 for Videocapturing via libv4l

    cv::setUseOptimized(1);

    cap.open(deviceID, apiID);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 2560);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    
    if( !cap.isOpened() )
    {
        cout << "Cannot open camera" <<std::endl;

        return -1;
    }

    if(cv::useOptimized()){

	cout<< "Optimized code enabled..." << std::endl;

    }else

        cout<< "Optimized code disabled..." << std::endl;




    int counter = 0;

    while(counter < num_frames){

        double start = (double)cv::getTickCount();

	    cap >> img;
        
        //Zed has 2 cameras, extract only one of the pictures
        cv::Mat left_img = img(cv::Rect(0, 0, img.cols / 2, img.rows));
        out = cv::Mat::zeros(left_img.size(), CV_8UC1);

        auto start1 = high_resolution_clock::now();
        cv::parallel_for_(cv::Range(0, 8), Parallel_process(left_img, out,8));
        //image_processing(left_img,out);
        auto stop1 = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop1-start1);
        auto time = duration.count()/1000;

        cout << "Time taken: "<< time << "ms" << endl; 
        cv::imshow("Edge Detection", out);
	    counter++;

        double end = (double)cv::getTickCount();

        cout << "Fps: "<<(cv::getTickFrequency()/ (end-start))<<std::endl;

        if (cv::waitKey(1) >= 0)
             break;
    }




  return 0;
}

