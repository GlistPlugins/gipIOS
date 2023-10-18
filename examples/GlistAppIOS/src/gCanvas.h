 /*
 * gCanvas.h
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#ifndef GCANVAS_H_
#define GCANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gIOSCanvas.h"


class gCanvas : public gIOSCanvas {
public:
	gCanvas(gApp* root);
	virtual ~gCanvas();

	void setup();
	void update();
	void draw();

	void windowResized(int w, int h);
    void touchMoved(int x, int y, int fingerId);
    void touchPressed(int x, int y, int fingerId);
    void touchReleased(int x, int y, int fingerId);

	void showNotify();
	void hideNotify();
    
    void pause();
    void resume();

private:
	gApp* root;
};

#endif /* GCANVAS_H_ */
