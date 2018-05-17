#include "imagelable.h"

// destructor
ImageLable::~ImageLable() {

}


void ImageLable::mouseReleaseEvent(QMouseEvent *evt)
{
    emit MouseRelease(evt);
}


void ImageLable::keyPressEvent(QKeyEvent *evt) {
    // listen to key press event,useless
    emit KeyPressed(evt);
}


void ImageLable::mouseMoveEvent(QMouseEvent *event) {
    emit MouseMoved(event);
}


void ImageLable::mousePressEvent(QMouseEvent *event) {
    emit MousePressed(event);
}


void ImageLable::mouseDoubleClickEvent(QMouseEvent *event) {
    emit MouseDoubliClick(event);
}

void ImageLable::wheelEvent(QWheelEvent *event) {
    emit WheelEvent(event);
}
