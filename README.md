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
g++ -c -m64 -pipe -O2 -std=c++0x -Wall -W -fPIC -I/usr/local/opencv-2.4.10/include -I/usr/local/opencv-2.4.10/include/opencv -I/usr/local/opencv-2.4.10/include/opencv2 -o bin/main.o main.cpp
g++ -m64 -Wl,-O1 -o bin/cardetect bin/main.o -L/usr/local/opencv-2.4.10/lib -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_objdetect
```
**Windows**

you need to install Micsoft Visual Studio
i provide a Visual Studio 2015 solution in [ project folder](https://github.com/tangchent/cardetect) (include OpenCV library),you can download and build it.

\###**Run Step**

**Linux**
``` bash
cd bin/
./cardetect
```
**Windows**

you can download [x64.zip file](https://github.com/tangchent/cardetect/blob/master/bin/x86.rar) ,if you use x64 system,download [x86.zip file](https://github.com/tangchent/cardetect/blob/master/bin/x86.rar) if you use x86 system.unzip the file, and you can run cardetect.exe directly if you hava DirectX 11 run enviroment.
