/*
 * gScrollTestScenario.h
 *
 * A GUI page used to exercise the mobile scrollable layout by hand: page
 * scrolling, the scroll indicator, the fling, and the still unsolved case of a
 * scrollable control living inside a scrolling page.
 *
 * Built on purpose out of ordinary controls only, so that what it shows is the
 * behaviour a real app would get rather than a special case.
 */

#ifndef GSCROLLTESTSCENARIO_H_
#define GSCROLLTESTSCENARIO_H_

#include "gApp.h"


class gScrollTestScenario {
public:
	// Fills the app's current GUI frame with the test page. Call once, from
	// setup(), after the frame exists.
	static void build(gApp* root);

	// Forward the canvas's onGuiEvent here so the page can show which taps
	// actually registered.
	static void onGuiEvent(int guiObjectId, int eventType);
};

#endif /* GSCROLLTESTSCENARIO_H_ */
