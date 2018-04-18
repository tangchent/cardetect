# car detection

implements Vehicle detection through cascade classifier.The cascade classifier is trained by OpenCV's demon app "opencv_traincascade"

![image](https://github.com/tangchent/cardetect/raw/master/picture/Picture1.png)

you can get the binary in the [bin folder](https://github.com/tangchent/cardetect/tree/master/bin "bin folder")

I will provide execute for windows and linux,and here are some **Requirements**

**Linux:**
    
    Requirements:
      1) GCC 5.x
         From: http://gcc.gnu.org/releases.html
         Or via apt:
         sudo apt-get install g++
      2) GTK+
         sudo apt-get install libgtk2.0-dev
      2) libpng12-dev
         sudo apt-get install libpng12-dev
      3) OpenCV 2.4.10 library
         From: http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/2.4.10/opencv-2.4.10.zip/download
         you need to compile the source code and install opencv library

**Windows:**

    Requirements:
      1) DirectX 11
        From: https://www.microsoft.com/en-us/download/details.aspx?id=8109
      2) MSVC 14.0
        From: https://www.microsoft.com/en-us/download/details.aspx?id=48145
      3) MSVC 12.0
      	From: https://www.microsoft.com/en-us/download/details.aspx?id=40784
\###**Build Step**

**Linux**

for 64 - bit system, assume you hava already install opencv in /usr/local/opencv-2.4.10/
``` bash
cd current directory
make
```
**Windows**

you need to install Micsoft Visual Studio
i provide a Visual Studio 2015 solution in [ project folder](https://github.com/tangchent/cardetect) (include OpenCV library),you can download and build it.

\###**Run Step**

**Linux**
``` bash
cd bin/
./objectdetect
```
By default,read the images from the current directory *data* and load the classifier from *cascade. xml*.
You can use thsi options:
>-v: read video files.

>-d: read the file from the directory, note that the directory can only contain image files, default:./data/.

>-c: loaded cascade classifier file name, default: cascade. xml.

>-cam: open camera.
``` bash
./objectdetect  [-v videofilename] [-c cascadename] [-d datadirctory] [-cam [id]]
```
**examples:**
read frame from video
``` bash
cd bin/
./objectdetect -v video1.avi
```
read frame from file
``` bash
cd bin/
./objectdetect -d data/
```
load specific cascade
``` bash
cd bin/
./objectdetect -d data/ -c cascade1.xml
```
read frame from camera ,default open 0
``` bash
cd bin/
./objectdetect -cam  1 -c cascade1.xml
```
**Windows**

you can download [x64.zip file](https://github.com/tangchent/cardetect/blob/master/bin/x86.rar) ,if you use x64 system,download [x86.zip file](https://github.com/tangchent/cardetect/blob/master/bin/x86.rar) if you use x86 system.unzip the file, and you can run cardetect.exe directly if you hava DirectX 11 run enviroment.


\###**Key Instructions **

key 's' : Stop/Continue auto play,only aviliable in read from file or video

key 'a' : Back 1 frame/picture,only aviliable in read from file or video

key 'd' : Forward 1 frame/picture,only aviliable in read from file or video

key 'k' : Back 60 frames / 5 pictures,only aviliable in read from file or video

key 'l ' : Forward 60 frames / 5 pictures,only aviliable in read from file or video

key 'ESC' : exit
