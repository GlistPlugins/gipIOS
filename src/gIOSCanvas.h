#ifndef GIOSCANVAS_H_
#define GIOSCANVAS_H_

#include "gBaseCanvas.h"

class gIOSCanvas : public gBaseCanvas
{
public:
    gIOSCanvas(gBaseApp* root);
    virtual ~gIOSCanvas();

    virtual void deviceOrientationChanged(DeviceOrientation deviceorientation);

    virtual void touchMoved(int x, int y, int fingerId);
    virtual void touchPressed(int x, int y, int fingerId);
    virtual void touchReleased(int x, int y, int fingerId);

    virtual void pause();
    virtual void resume();
};

#endif // GIOSCANVAS_H_
