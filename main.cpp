#include <iostream>
#include <vector>
#include <stdio.h>
#include <time.h>
#include "objectdetect.h"

using namespace std;


/** @function main */
int main(int argc, char** argv)
{
    char c;
    int step = 1;
    bool stop = false;
    string window_name = "Object Detection";
    Mat frame;
    ObjectDetect * objectDetect = new ObjectDetect();

    if (!objectDetect->readParameters(argc,argv)) {
        printf("some parameters read failed,now use default\n");
    }
    if (!objectDetect->init()) {
        return -1;
    }

    //message and create window
    printf("press ESC to exit\n");
    printf("press S   to stop\n");
    namedWindow(window_name,1);

    while (true) {
        c = waitKey(10);
        if (c == 's' || c == 'S') { //stop
            stop = !stop;
        }
        if (c == 27) { //ESC
            break;
        } else if (c == 'a' || c == 'A' || c == 81) { //A or left arrow
            frame = objectDetect->loadFrame(-1);
        } else if (c == 'd' || c == 'D' || c == 83) { //D or right arrow
            frame = objectDetect->loadFrame(1);
        } else if (c == 'k' || c == 'K') { //big step
            if (objectDetect->isFromFile()) {
                step = -5;
            } else if (objectDetect->isFromVideo()) {
                step = -60;
            }
            frame = objectDetect->loadFrame(step);
        } else if (c == 'l' || c == 'L') { //big step
            if (objectDetect->isFromFile()) {
                step = 5;
            } else if (objectDetect->isFromVideo()) {
                step = 60;
            }
            frame = objectDetect->loadFrame(step);
        } else {
            if (stop){
                step = 0;
            } else {
                step = 1;
            }
            frame = objectDetect->loadFrame(step);
        }

        if (frame.empty()) {
            printf("\033[0;31mno frame input!\033[0m\n");
            break;
        }
        // start time
        const clock_t begin_time = clock();
        //detect and display
        objectDetect->detectObjects(frame);
        // end time
        float seconds = float(clock() - begin_time) / CLOCKS_PER_SEC;

        for (size_t i = 0; i < objectDetect->objects.size(); i++) {
            //draw rectangle
            rectangle(frame, objectDetect->objects[i], Scalar(0, 0, 255));
        }
        imshow(window_name, frame);

#if defined WIN32 || defined _WIN32
        if (objectDetect->isFromFile()) {
            fprintf(stdout,"filename: %s  time cost: %.3f s\r", objectDetect->getFilename(), seconds);
        } else if (objectDetect->isFromVideo()) {
            fprintf(stdout,"videoname: %s  time cost: %.3f s\r", objectDetect->getVideoname(), seconds);
        }
#else
        if (objectDetect->isFromFile()) {
            //colorful print
            fprintf(stdout,"filename : \033[0;36m%s\033[0m\ntime cost: \033[0;32m%.3f\033[0m s\r\n\33[2A", objectDetect->getFilename(), seconds);
        } else if (objectDetect->isFromVideo()) {
            //colorful print
            fprintf(stdout,"videoname : \033[0;36m%s\033[0m\ncurrent frame : \033[0;36m%d\033[0m time cost: \033[0;32m%.3f\033[0m s\r\n\33[2A", objectDetect->getVideoname(), objectDetect->getCurrentFrame(), seconds);
        } else if (objectDetect->isFromCamera()) {
            //colorful print
            fprintf(stdout,"from camera, time cost: \033[0;32m%.3f\033[0m s\r", seconds);
        }
#endif
        fflush(stdout);
    }

    //release object
    delete objectDetect;
    //release window
    destroyWindow(window_name);

#if defined WIN32 || defined _WIN32
    fprintf(stdout,"\nexit...\n");
#else
    //   \33[1B         move cursor 1 line down
    //   \033[0;31m     set color RED
    //   \033[0m        set off
    printf("\33[1B\n\033[1;31mexit...\033[0m\n");
#endif
    return 0;
}
