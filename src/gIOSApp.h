#ifndef GIOSAPP_H_
#define GIOSAPP_H_

#include "gBaseApp.h"

class gIOSApp : public gBaseApp
{
public:
    gIOSApp();
    virtual ~gIOSApp();

    virtual void pause();
    virtual void resume();
};

#endif // GIOSAPP_H_
