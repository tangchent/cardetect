#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay(Mat frame);
void executeCMD(const char *cmd, vector<String *>& result);

/** Global variables */
String car_cascade_name = "cascade.xml";
CascadeClassifier car_cascade;
String window_name = "Car detection";

/** @function main */
int main(int argc, const char** argv)
{
    vector<String *> result(1024);
    Mat frame;
    int index = 0;
    char c;
    char filename[256];
    bool stop = false;

    //message and create window
    printf("press ESC to exit\n");
    namedWindow(window_name,1);

    //-- 1. Load the cascades
    if (!car_cascade.load(car_cascade_name)) {
        printf("--(!)Error loading '%s'\n", car_cascade_name.c_str());
        return -1;
    }

    //-- 2. Read the video stream
//    CvCapture* capture;
//    capture = cvCaptureFromCAM(0);
//    if (capture)
//    {
//        while (true)
//        {
//            frame = cvQueryFrame(capture);

//            //-- 3. Apply the classifier to the frame
//            if (!frame.empty())
//            {
//                detectAndDisplay(frame);
//            }
//            else
//            {
//                printf(" --(!) No captured frame -- Break!"); break;
//            }

//            int c = waitKey(10);
//            if ((char)c == 'c') {
//                destroyWindow(window_name);
//                cvReleaseCapture(&capture);
//                break;
//            }
//        }
//    }

    //-- 2. Read the picture stream
    result.clear();
    executeCMD("ls data/*.png",result); //get file name

    while (true) {
        //remove \n
        strncpy(filename,result.at(index)->c_str(),result.at(index)->size() - 1);
        //open file
        frame = imread(filename);
        if(frame.empty()) {
            break;
        }

        // start time
        const clock_t begin_time = clock();
        //detect and display
        detectAndDisplay(frame);
        // end time
        float seconds = float( clock () - begin_time ) / CLOCKS_PER_SEC;
        fprintf(stdout,"time cost: %.3f s\r",seconds);
        fflush(stdout);

        c = waitKey(10);
        if (!stop && ++index > (int)(result.size() - 1))    index = 0;
        if (c == 27) { //ESC
            break;
        } else if (c == 'a' || c == 'A' || c == 81) { //A or left arrow
            if (--index <  0)    index = 0;
            frame = imread(*result.at(index));
        } else if (c == 'd' || c == 'D' || c == 83) { //D or right arrow
            if (++index > (int)(result.size() - 1))    index = 0;
            frame = imread(*result.at(index));
        } else if (c == 's' || c == 'S') {
            stop = !stop;
        }
    }

    for (vector<String *>::iterator itr = result.begin(); itr != result.end(); ++itr) {
        delete *itr;
    }

    //release window
    destroyWindow(window_name);

    printf("exit...\n");
    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay(Mat frame)
{
    std::vector<Rect> cars;
    Mat frame_gray;

    //-- Convert to gray iamge
    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    //-- normalizes the grayscale image brightness and contrast by normalizing its histogram
    equalizeHist(frame_gray, frame_gray);

    //-- Detect cars
    car_cascade.detectMultiScale(frame_gray, //Mat
                                 cars, //result
                                 1.2, //scaleFactor
                                 3, //minNeighbors
                                 CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_ROUGH_SEARCH | CV_HAAR_FIND_BIGGEST_OBJECT, //flags
                                 Size(32, 32), //minSize
                                 Size()); //maxSize

    for (size_t i = 0; i < cars.size(); i++)
    {
        //draw rectangle
        rectangle(frame, cars[i], Scalar(0, 0, 255));
    }
    //-- Show what you got
    imshow(window_name, frame);
}

void executeCMD(const char *cmd, vector<String *>& result)
{
    char buf_ps[1024];
    char ps[1024];
    FILE * ptr;
    String * str = NULL;
    strcpy(ps, cmd);
    if((ptr = popen(ps, "r")) != NULL) {
        while(fgets(buf_ps, 1024, ptr) != NULL) {
            str = new String(buf_ps);
            if (str != NULL) {
                result.push_back(str);
            } else {
                break;
            }
            str = NULL;
        }
        pclose(ptr);
        ptr = NULL;
    }
    else {
        printf("popen '%s' error\n", ps);
    }
}
