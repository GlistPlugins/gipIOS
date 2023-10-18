/*
* gCanvas.cpp
*
*  Created on: May 6, 2020
*      Author: noyan
*/


#include "gCanvas.h"


gCanvas::gCanvas(gApp* root) : gIOSCanvas(root) {
	this->root = root;
}

gCanvas::~gCanvas() {
}

void gCanvas::setup() {
}

void gCanvas::update() {
}

void gCanvas::draw() {

}

void gCanvas::windowResized(int w, int h) {

}

void gCanvas::showNotify() {

}

void gCanvas::hideNotify() {

}

void gCanvas::touchMoved(int x, int y, int fingerId)
{
    gLogi("touchMoved") << "x: " << x << " y: " << y << " fingerid: " << fingerId;
}

void gCanvas::touchPressed(int x, int y, int fingerId)
{
    gLogi("touchPressed") << "x: " << x << " y: " << y << " fingerid: " << fingerId;
}

void gCanvas::touchReleased(int x, int y, int fingerId)
{
    gLogi("touchReleased") << "x: " << x << " y: " << y << " fingerid: " << fingerId;
}

void gCanvas::pause()
{
    gLogi("pause");
}

void gCanvas::resume()
{
    gLogi("resume");
}
