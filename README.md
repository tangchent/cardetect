# car detection

implements Vehicle detection through cascade classifier.The cascade classifier is trained by OpenCV's demon app "opencv_traincascade"

![image](https://github.com/tangchent/cardetect/raw/master/picture/Picture1.png)

you can get the binary in the [bin folder](https://github.com/tangchent/cardetect/tree/master/bin "bin folder")

I will provide execute for windows and linux,and here are some Requirements

**Linux:**
    
    Requirements:
      1) GCC 5.x
         From: http://gcc.gnu.org/releases.html
         Or via apt:
         sudo apt-get install g++
      2) libpng12-dev
         sudo apt-get install libpng12-dev
      3) OpenCV 2.4.10 library
         From: http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/2.4.10/opencv-2.4.10.zip/download
         you need to compile the source code and install opencv library

**Windows:**

    Requirements:
      1) DirectX 11
        From: https://www.microsoft.com/en-us/download/details.aspx?id=8109
      2) MSVC14.0
        From: https://www.microsoft.com/en-us/download/details.aspx?id=48145
\###**Build Step**
**Linux**

for 64 - bit system, assume you hava already install opencv in /usr/local/opencv-2.4.10/
``` bash
cd current directory
g++ -c -m64 -pipe -O2 -std=c++0x -Wall -W -fPIC -I/usr/local/opencv-2.4.10/include -I/usr/local/opencv-2.4.10/include/opencv -I/usr/local/opencv-2.4.10/include/opencv2 -o bin/main.o main.cpp
g++ -m64 -Wl,-O1 -o bin/cardetect bin/main.o -L/usr/local/opencv-2.4.10/lib -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_objdetect
```
\###**Run Step**
**Linux**
``` bash
cd bin/
./cardetect
```
