#include "objectdetect.h"
#include <stdio.h>

ObjectDetect::ObjectDetect():
    index(0),
    currentFrame(0),
    stop(false),
    fromFile(false),
    fromVideo(false),
    fromCamera(false),
    initFinish(false)
{
    imageFilenames.resize(1024);
}

ObjectDetect::~ObjectDetect()
{
    for (vector<string *>::iterator itr = imageFilenames.begin(); itr != imageFilenames.end(); ++itr) {
        delete *itr;
    }
    if (cap.isOpened()) {
        cap.release();
    }
}

/**
 * get arguments from command line
 * @param argc. arguments count
 * @param argv.point to arguments array
 *
 * parameters see usage
 * @return
 * true if read success
 * false if failed
 */
bool ObjectDetect::readParameters(int argc, char **argv)
{
    for (int i = 1;i < argc; ++i) {
        if (strcmp(argv[i],"-v") == 0 && !fromFile && !fromCamera) {
            if (++i < argc) {
                strcpy(videoname,argv[i]);
                fromVideo = true;
            } else {
                usage();
                return false;
            }
        }
        if (strcmp(argv[i], "-c") == 0) {
            if (++i < argc) {
                strcpy(objectCascadeName, argv[i]);
            }
            else {
                usage();
                return false;
            }
        }
        if (strcmp(argv[i], "-d") == 0 && !fromVideo && !fromCamera) {
            if (++i < argc) {
                strcpy(filedir, argv[i]);
                fromFile = true;
            }
            else {
                usage();
                return false;
            }
        }
        if (strcmp(argv[i], "-cam") == 0 && !fromVideo && !fromFile) {
            if (++i < argc) {
                cameraDevice = atoi(argv[i]);
            }
            fromCamera = true;
        }
    }
    if (!fromFile && !fromVideo && !fromCamera) {
        fromFile = true;
    }
    return true;
}

/**
 * init
 * @param void
 * @return void
 */
bool ObjectDetect::init( void )
{
    bool result = true;
    index = 0;
    currentFrame = 0;
    if (!objectCascadeClassifier.load(objectCascadeName)) {
        printf("--(!)Error loading '%s'\n", objectCascadeName);
        result = false;
    } else {
        printf("classifier: '%s' loaded\n", objectCascadeName);
    }

    if (fromFile) {
        readImageSequenceFiles(filedir);
        printf("image directory: '%s', total files: %d\n", filedir, imageFilenames.size());
    } else if (fromVideo) {
        cap.open(videoname);
        if (!cap.isOpened()) {
            printf("--(!)Error loading '%s'\n", videoname);
            result = false;
        }
    } else if (fromCamera) {
        cap.open(cameraDevice);
        if (!cap.isOpened()) {
            printf("--(!)Error open device number:%d\n", cameraDevice);
            result = false;
        }
    } else {
        readImageSequenceFiles(filedir);
        printf("image directory: '%s'\n", filedir);
    }
    initFinish = result;
    return result;
}
/**
 * print help messages
 * @param void
 * @return void
 */
inline void ObjectDetect::usage( void )
{
    printf("-v video filname: read from video stream.\n");
    printf("-c cascade classifier name: load specifical cascade classifier.\n");
    printf("-d data directory: read image from data directory.\n");
    printf("-cam video device name: read image from camera.\n");
}


#if defined WIN32 || defined _WIN32
/**
* get all filename in given directory
* only for windows platform
* @param imgFilePath. the file directory
* @param result . the reference of result.
* @return
*/
void ObjectDetect::readImageSequenceFiles(char * path)
{
    imageFileames.clear();

    string * str = NULL;
    char tmpDirSpec[MAX_PATH + 1];
    sprintf(tmpDirSpec, "%s\\*", path);

    WIN32_FIND_DATA f;
    HANDLE h = FindFirstFile(tmpDirSpec, &f);
    if (h != INVALID_HANDLE_VALUE)
    {
        FindNextFile(h, &f);	//read ..
        FindNextFile(h, &f);	//read .
        do
        {
            str = new string(f.cFileName);
            if (str != NULL) {
                imageFilenames.push_back(str);
            }
        } while (FindNextFile(h, &f));

    }
    FindClose(h);
}
#else
/**
* execute command ls *
* get all filename in given directory
* only for linux platform , windows is not supported!
* @param cmd. the command name needed to be executed
* only support linux commands
* @param result . the reference of command result.
* if execute 'ls' ,the result contains all file name in current dir
* attention that result is split by '\n',and each element contains '\n'
* @return
*/
void ObjectDetect::readImageSequenceFiles( char * path )
{
    char buf_ps[1024];
    char ps[1024];
    char cmd[256];
    int len;
    FILE * ptr;
    string * str = NULL;

    imageFilenames.clear();

    strcpy(cmd, "ls ");
    strcat(cmd, path);
    len = strlen(cmd);
    if (*(cmd + len - 1) != '/') {
        *(cmd + len) = '/';
        *(cmd + len + 1) = '*';
        *(cmd + len + 2) = '\0';
    } else {
        *(cmd + len) = '*';
        *(cmd + len + 1) = '\0';
    }
    strcpy(ps, cmd);
    if ((ptr = popen(ps, "r")) != NULL) {
        while (fgets(buf_ps, 1024, ptr) != NULL) {
            str = new string(buf_ps);
            if (str != NULL) {
                imageFilenames.push_back(str);
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

/**
 * loadNextFrame  function
 *
 * @param step . jump step of image sequence or video frames
 * @return Mat  the image readed;
 */
Mat ObjectDetect::loadFrame(int step)
{
    Mat frameRead;
    if (fromFile) {
        if (index < 0) index = (int)(imageFilenames.size() - 1);
        if (index > (int)(imageFilenames.size() - 1)) index = 0;
        if (imageFilenames.size() == 0) {
            return frameRead;
        } else {
#if defined WIN32 || defined _WIN32
            //this string don't contain FILR_DIR,so need be copy.
            int len = imageFilenames.at(index)->size();
            strcpy(filename, filedir);
            strcat(filename, "\\");
            strcat(filename, imageFilenames.at(index)->c_str());
#else
            strncpy(filename,imageFilenames.at(index)->c_str(),imageFilenames.at(index)->size());
            //remove '\n'
            *(filename + imageFilenames.at(index)->size() - 1) = '\0';
#endif
            index += step;
//            printf("#%s#",filename);
            //read file
            frameRead = imread(filename);
            return frameRead;
        }
    } else if (fromVideo) {
        if (currentFrame < 0)	currentFrame = 0;
        if (currentFrame > (cap.get(CV_CAP_PROP_FRAME_COUNT) - 1))	currentFrame = 0;
        cap.set(CV_CAP_PROP_POS_FRAMES, (double)currentFrame);
        cap >> frameRead;
        currentFrame += step;
        //try to restart
        if(frameRead.empty()) {
            cap.set( CV_CAP_PROP_POS_FRAMES,0);
            cap >> frameRead;
            currentFrame = 1;
        }
    } else if (fromCamera) {
        cap >> frameRead;
    }
    return frameRead;
}

/**
 * detect  function
 *
 * @param frame . the image needed to detection,the format should be BGR
 * @return int  the number of the detected objects
 */
int ObjectDetect::detectObjects(Mat frame)
{
    Mat frame_gray;

    objects.clear();

    //-- Convert to gray iamge
    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    //-- normalizes the grayscale image brightness and contrast by normalizing its histogram
    equalizeHist(frame_gray, frame_gray);

    //-- Detect objects
    objectCascadeClassifier.detectMultiScale(frame_gray, //Mat
                                 objects, //result
                                 1.2, //scaleFactor
                                 3, //minNeighbors
                                 0 | CV_HAAR_SCALE_IMAGE,
                                 //CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_ROUGH_SEARCH | CV_HAAR_FIND_BIGGEST_OBJECT, //flags,actually not used yet?
                                 Size(12, 12), //minSize
                                 Size()); //maxSize

    return objects.size();
}

/**
 * freeFilenames  function
 *
 * @param
 * @return
 */
void ObjectDetect::freeFilenames()
{
    for (vector<string *>::iterator itr = imageFilenames.begin(); itr != imageFilenames.end(); ++itr) {
        delete *itr;
    }
    initFinish = false;
}

/**
 * closeVideo  function
 *
 * @param
 * @return
 */
void ObjectDetect::closeCapture()
{
    if (cap.isOpened()) {
        cap.release();
    }
    initFinish = false;
}

/**
 * get  function
 *
 * @param
 * @return bool  fromFile
 */
bool ObjectDetect::isFromFile()
{
    return fromFile;
}

/**
 * get  function
 *
 * @param
 * @return bool  fromVideo
 */
bool ObjectDetect::isFromVideo()
{
    return fromVideo;
}

/**
 * get  function
 *
 * @param
 * @return bool  fromCamera
 */
bool ObjectDetect::isFromCamera()
{
    return fromCamera;
}

/**
 * get  function
 *
 * @param
 * @return bool  initFinish
 */
bool ObjectDetect::isInitialized()
{
    return initFinish;
}

/**
 * set  function
 *
 * @param bool val
 * @return void
 */
void ObjectDetect::setFromCamera(bool val)
{
    fromCamera = val;
}

/**
 * set  function
 *
 * @param bool val
 * @return void
 */
void ObjectDetect::setFromVideo(bool val)
{
    fromVideo = val;
}

/**
 * set  function
 *
 * @param bool val
 * @return void
 */
void ObjectDetect::setFromFile(bool val)
{
    fromFile = val;
}

/**
 * get  function
 *
 * @param
 * @return char *  file directory
 */
char * ObjectDetect::getFileDirectory()
{
    return filedir;
}

/**
 * get  function
 *
 * @param
 * @return char *  filename
 */
char * ObjectDetect::getFilename()
{
    return filename;
}

/**
 * get  function
 *
 * @param
 * @return char *  videoname
 */
char * ObjectDetect::getVideoname()
{
    return videoname;
}

/**
 * get  function
 *
 * @param
 * @return char *  VideoFrames
 */
int    ObjectDetect::getVideoFrames()
{
    if (fromVideo) {
        return cap.get(CV_CAP_PROP_FRAME_COUNT);
    } else {
        return -1;
    }
}

/**
 * get  function
 *
 * @param
 * @return char *  videoname
 */
int    ObjectDetect::getFileCount()
{
    if (fromFile) {
        return imageFilenames.size();
    } else {
        return -1;
    }
}

/**
 * get  function
 *
 * @param
 * @return char *  objectCascadeName
 */
char * ObjectDetect::getCascadeName()
{
    return objectCascadeName;
}

/**
 * get  function
 *
 * @param
 * @return int camera device index
 */
int    ObjectDetect::getCameraDevice()
{
    return cameraDevice;
}

/**
 * get  function
 *
 * @param
 * @return currentFrame
 */
int ObjectDetect::getCurrentFrame()
{
    return currentFrame;
}

/**
 * get  function
 *
 * @param
 * @return index
 */
int ObjectDetect::getCurrentIndex()
{
    return index;
}

/**
 * set  function
 *
 * @param
 * @return true if copy success
 */
bool ObjectDetect::setObjectCascadeName( const char * name )
{
    if (strlen(name) > (BUFFER_LENGTH - 1)) return false;
    strcpy(objectCascadeName,name);
    return true;
}

/**
 * set  function
 *
 * @param
 * @return true if copy success
 */
bool ObjectDetect::setFileDirectory( const char * name )
{
    if (fromVideo || fromCamera) {
        return false;
    }
    if (strlen(name) > (BUFFER_LENGTH - 1)) {
        return false;
    }
    strcpy(filedir,name);
    return true;
}

/**
 * set  function
 *
 * @param
 * @return true if copy success
 */
bool ObjectDetect::setVideoname( const char * name )
{
    if (fromFile || fromCamera) {
        return false;
    }
    if (strlen(name) > (BUFFER_LENGTH - 1)) {
        return false;
    }
    strcpy(videoname,name);
    fromVideo = true;
    return true;
}

/**
 * set  function
 *
 * @param
 * @return true if copy success
 */
bool ObjectDetect::setCameraDevice(int idx) {
    cameraDevice = idx;
    return true;
}
