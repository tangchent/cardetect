#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <vector>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#include <time.h>
#endif // _WIN32


using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay(Mat frame);
#if _WIN32
#define FILE_DIR	".\\data\\"
void readImageSequenceFiles(char* imgFilePath, vector<String *>& result);
#else
void executeCMD(const char *cmd, vector<String *>& result);
#endif
/** Global variables */
#ifdef _WIN32
std::vector<Rect> cars;
#endif
String car_cascade_name = "cascade.xml"; //the cascades file name
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

    //-- 2. Read the picture stream
    result.clear();
#if _WIN32
	readImageSequenceFiles(FILE_DIR, result);
#else
    executeCMD("ls data/*.png",result); //get file name
#endif

    while (true) {
#ifdef _WIN32
		int len = result.at(index)->size();
		strcpy(filename, FILE_DIR);
		strncpy(filename + strlen(FILE_DIR), result.at(index)->c_str(), len);
		*(filename + strlen(FILE_DIR) + len) = '\0';
#else
        //remove '\n'
        strncpy(filename,result.at(index)->c_str(),result.at(index)->size() - 1);
#endif
		//open file
        frame = imread(filename);
        if(frame.empty()) {
             printf(" --(!) No frame -- Break!");
             break;
        }

        // start time
		const clock_t begin_time = clock();
		//detect and display
		detectAndDisplay(frame);
		// end time
		float seconds = float(clock() - begin_time) / CLOCKS_PER_SEC;

#ifdef _WIN32
        fprintf(stdout,"filename: %s  time cost: %.3f s\r", filename, seconds);
#else
        //colorful print
        fprintf(stdout,"filename : \033[0;36m%s\033[0m\ntime cost: \033[0;32m%.3f\033[0m s\r\n\33[2A", filename, seconds);
#endif
        fflush(stdout);

        c = waitKey(10);
        if (!stop && ++index > (int)(result.size() - 1))    index = 0; //auto read
        if (c == 27) { //ESC
            break;
        } else if (c == 'a' || c == 'A' || c == 81) { //A or left arrow
            if (--index <  0)    index = 0;
        } else if (c == 'd' || c == 'D' || c == 83) { //D or right arrow
            if (++index > (int)(result.size() - 1))    index = 0;
        } else if (c == 's' || c == 'S') {
            stop = !stop;
        }
    }

    for (vector<String *>::iterator itr = result.begin(); itr != result.end(); ++itr) {
        delete *itr;
    }

    //release window
    destroyWindow(window_name);


#ifdef _WIN32
    fprintf(stdout,"\nexit...\n");
#else
    //   \33[1B         move cursor 1 line down
    //   \033[0;31m     set color RED
    //   \033[0m        set off
    printf("\33[1B\n\033[0;31mexit...\033[0m\n");
#endif
    return 0;
}
/**
 * detect and display function
 *
 * @param frame . the image needed to detection,the format should be BGR
 * @return
 */
void detectAndDisplay(Mat frame)
{
#ifndef  _WIN32
	std::vector<Rect> cars;
#endif // ! _WIN32
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

    for (size_t i = 0; i < cars.size(); i++) {
        //draw rectangle
        rectangle(frame, cars[i], Scalar(0, 0, 255));
    }
    //-- Show what you got
	imshow(window_name, frame);
}

#ifdef _WIN32
void readImageSequenceFiles(char* imgFilePath, vector<String *>& result)
{
	result.clear();

	String * str = NULL;
	char tmpDirSpec[MAX_PATH + 1];
	sprintf(tmpDirSpec, "%s/*", imgFilePath);

	WIN32_FIND_DATA f;
	HANDLE h = FindFirstFile(tmpDirSpec, &f);
	if (h != INVALID_HANDLE_VALUE)
	{
		FindNextFile(h, &f);	//read ..
		FindNextFile(h, &f);	//read .
		do
		{
			str = new String(f.cFileName);
			if (str != NULL) {
				result.push_back(str);
			}
		} while (FindNextFile(h, &f));

	}
	FindClose(h);
}
#else
/**
* execute command use command line
* only for linux platform , windows is not supported!
* @param cmd. the command name needed to be executed
* only support linux commands
* @param result . the reference of command result.
* if execute 'ls' ,the result contains all file name in current dir
* attention that result is split by '\n',and each element contains '\n'
* @return
*/
void executeCMD(const char *cmd, vector<String *>& result)
{
	char buf_ps[1024];
	char ps[1024];
	FILE * ptr;
	String * str = NULL;
	strcpy(ps, cmd);
	if ((ptr = popen(ps, "r")) != NULL) {
		while (fgets(buf_ps, 1024, ptr) != NULL) {
			str = new String(buf_ps);
			if (str != NULL) {
				result.push_back(str);
			}
			else {
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
#endif