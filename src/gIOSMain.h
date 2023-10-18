/*
 * gIOSEntryPoint.h
 *
 *  Created on: Sep 2, 2023
 *      Created by: kayra
 */

#ifndef G_IOSMAIN_H
#define G_IOSMAIN_H

#include <iosfwd>

int ios_main(void* baseApp, const char* appName, int windowMode, int unitWidth, int unitHeight, int screenScaling, int width, int height, bool isResizable);

#endif /* G_IOSMAIN_H */
