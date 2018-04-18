#ifndef OBJECTDETECT_H
#define OBJECTDETECT_H
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include <vector>

#if defined WIN32 || defined _WIN32
#define FILE_DIR        ".\\data\\"
#define CAMERA_DEVICE	"0"
#else
#define FILE_DIR        "./data"
#define CAMERA_DEVICE	"/dev/video0"
#endif
#define CASCADE_NAME	"cascade.xml"


using namespace cv;
using namespace std;

class ObjectDetect
{
public:
    ObjectDetect();
    ~ObjectDetect();
    bool readParameters(int argc, char** argv);
    bool init( void );
    inline void usage( void );

#if defined WIN32 || defined _WIN32
    void readImageSequenceFiles( void );
#else
    void readImageSequenceFiles( void );
#endif
    Mat loadFrame(int step = 1);
    int detectObjects(Mat frame);

    vector<Rect> objects;

private:
    CascadeClassifier objectCascadeClassifier;
    Mat frame;
    VideoCapture cap;
    vector<string *> imageFileames;
    int index;
    int currentFrame;
    char objectCascadeName[128] = CASCADE_NAME; //the cascades file name
    char filedir[256]           = FILE_DIR;
    char filename[256];
    char videoname[256];
    char cameraDevice[32]       = CAMERA_DEVICE;
    bool stop;
    bool fromFile;
    bool fromVideo;
    bool fromCamera;
};

#endif // OBJECTDETECT_H
