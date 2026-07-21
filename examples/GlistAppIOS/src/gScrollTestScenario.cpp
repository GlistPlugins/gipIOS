/*
 * gScrollTestScenario.cpp
 *
 *  See gScrollTestScenario.h.
 */

#include "gScrollTestScenario.h"

#include "gGUIManager.h"
#include "gGUIFrame.h"
#include "gGUISizer.h"
#include "gGUIText.h"
#include "gGUIButton.h"
#include "gGUIListbox.h"
#include "gGUINotebook.h"
#include "gGUITextbox.h"
#include "gGUINumberBox.h"
#include "gGUISlider.h"
#include "gGUISwitchButton.h"
#include "gGUIProgressBar.h"
#include "gGUIDropdownList.h"
#include "gGUITimebox.h"
#include "gGUIDate.h"

#include <string>


namespace {

// gGUIButton keeps the fixed 96x32 it is constructed with: the sizer positions it
// but never resizes it, and the title is centred inside that box rather than the
// box being grown to hold the title. A long title therefore spills out of the
// button. The size is set explicitly here, wide enough for the titles used and
// tall enough to be a comfortable finger target.
const int buttonwidth = 320;
const int buttonheight = 72;

// Enough buttons that their heights alone overrun the 1280 unit design height,
// so the page has no choice but to grow.
const int squashbuttoncount = 12;

// The counters live here rather than on the controls, because the buttons report
// their taps through onGuiEvent on the canvas and the text has to be reachable
// from there. A test page is allowed to be this blunt.
gGUIText* statustext = nullptr;
gGUIButton* buttona = nullptr;
gGUIButton* buttonb = nullptr;
gGUIButton* buttonc = nullptr;
int buttonataps = 0;
int buttonbtaps = 0;
int buttonctaps = 0;

void refreshStatus() {
	if(!statustext) return;
	statustext->setText("Dokunma: A=" + std::to_string(buttonataps) +
			"  B=" + std::to_string(buttonbtaps) +
			"  C=" + std::to_string(buttonctaps));
}

gGUIText* addText(gGUISizer* sizer, int line, const std::string& text) {
	gGUIText* control = new gGUIText();
	sizer->setControl(line, 0, control);
	control->setText(text);
	return control;
}

gGUIButton* addButton(gGUISizer* sizer, int line, const std::string& title) {
	gGUIButton* control = new gGUIButton();
	// Sized before it goes in, because the sizer measures a control the moment it
	// is added. Added first, the row would be laid out around the default 96x32
	// and the real 320x72 would then overflow it.
	control->setSize(buttonwidth, buttonheight);
	control->setTitle(title);
	sizer->setControl(line, 0, control);
	return control;
}

// The form controls below are here to prove one thing each: that the natural
// height they now report gives them a row of the right size and keeps them off
// their neighbours. None of them needs its taps counted - looking at the page is
// the test. The textbox and numberbox double as the soft-keyboard test: tapping
// them must raise the keyboard and typed text must land in the control.
gGUITextbox* addTextbox(gGUISizer* sizer, int line, const std::string& text) {
	gGUITextbox* control = new gGUITextbox();
	sizer->setControl(line, 0, control);
	control->setText(text);
	control->setEditable(true);
	return control;
}

gGUINumberBox* addNumberBox(gGUISizer* sizer, int line) {
	gGUINumberBox* control = new gGUINumberBox();
	sizer->setControl(line, 0, control);
	control->setType(true);
	control->setMinValue(0);
	control->setMaxValue(100);
	control->setInteger(42);
	return control;
}

gGUISlider* addSlider(gGUISizer* sizer, int line) {
	gGUISlider* control = new gGUISlider();
	// Ticks on, so the natural height has to include the room below the bar for
	// the tick numbers - the branch that would collapse if it read height.
	control->setTickVisibility(true);
	control->setTextVisibility(true);
	control->setMinValue(0);
	control->setMaxValue(100);
	sizer->setControl(line, 0, control);
	return control;
}

gGUISwitchButton* addSwitch(gGUISizer* sizer, int line) {
	gGUISwitchButton* control = new gGUISwitchButton();
	sizer->setControl(line, 0, control);
	return control;
}

gGUIProgressBar* addProgressBar(gGUISizer* sizer, int line, gGUIProgressBar::TYPE type) {
	gGUIProgressBar* control = new gGUIProgressBar();
	control->setType(type);
	control->setMinValue(0);
	control->setMaxValue(100);
	control->setValue(60);
	sizer->setControl(line, 0, control);
	return control;
}

gGUIDropdownList* addDropdown(gGUISizer* sizer, int line, gGUIFrame* frame) {
	gGUIDropdownList* control = new gGUIDropdownList();
	control->setParentForm(frame);
	sizer->setControl(line, 0, control);
	for(int i = 0; i < 5; i++) {
		gGUITreelist::Element* element = new gGUITreelist::Element();
		element->title = "Secenek " + std::to_string(i);
		control->addElement(element);
	}
	return control;
}

gGUITimebox* addTimebox(gGUISizer* sizer, int line) {
	gGUITimebox* control = new gGUITimebox();
	sizer->setControl(line, 0, control);
	return control;
}

gGUIDate* addDate(gGUISizer* sizer, int line) {
	gGUIDate* control = new gGUIDate();
	sizer->setControl(line, 0, control);
	return control;
}

}

void gScrollTestScenario::build(gApp* root) {
	gGUIFrame* frame = root->getGUIManager()->getCurrentFrame();
	gGUISizer* sizer = frame->getSizer();
	// The line proportions used to be set by hand here, because even shares left
	// the list and the buttons too short for their own contents. They are gone on
	// purpose: rows now take what they need and the page grows to hold them, which
	// is exactly what the bottom block below is here to prove.
	//
	// Row count: eight rows above the form block, ten in it (a label plus nine
	// controls), the squash buttons, and the bottom marker. The lines are handed
	// out by a running counter below rather than numbered by hand, so inserting a
	// row is a one-line change and can never collide with another.
	sizer->setSize(19 + squashbuttoncount, 1);
	// Borders make the slot boundaries visible, which is what tells you at a
	// glance how far the page has scrolled.
	sizer->enableBorders(true);

	int line = 0;

	// The two markers sit at the extremes of the page. Seeing the bottom one at
	// all is the proof that the overflow is reachable.
	addText(sizer, line++, "UST - sayfanin basi");

	// Counts every tap, so whether a press registered is not a matter of opinion.
	// A drag that turns into a scroll must leave these numbers alone.
	statustext = addText(sizer, line++, "");
	refreshStatus();

	buttona = addButton(sizer, line++, "BUTON A");

	// The nested scrolling case. Dragging inside this list scrolls the page
	// instead of the list, and the list has no content drag of its own, so on a
	// touch screen its rows below the visible ones cannot be reached at all.
	gGUIListbox* listbox = new gGUIListbox();
	sizer->setControl(line++, 0, listbox);
	listbox->setVisibleLineNumber(6);
	listbox->enableScrollbars(true, false);
	for(int i = 0; i < 40; i++) {
		listbox->addData("Liste satiri " + std::to_string(i));
	}

	addText(sizer, line++, "Listeyi parmakla kaydirmayi deneyin");

	// The second nested scrolling case, and a different one: what overflows here
	// is the tab strip, not the content. The titles are long and there are enough
	// of them that the strip cannot fit, so the last tabs are reachable only by
	// dragging the strip sideways. BUTON C sits inside a tab, which is also the
	// check that controls placed in a tab body receive touches at all.
	gGUINotebook* notebook = new gGUINotebook();
	sizer->setControl(line++, 0, notebook);
	for(int i = 0; i < 8; i++) {
		gGUISizer* tabsizer = new gGUISizer();
		tabsizer->setSize(2, 1);
		int tabindex = notebook->addTab(tabsizer, "Sekme basligi " + std::to_string(i), false);
		addText(tabsizer, 0, "Bu " + std::to_string(tabindex) + ". sekmenin icerigi");
		// On the tab that opens first, so it is seen without having to switch tabs
		// - it is the check that a control inside a tab body is reachable at all,
		// and that check should not depend on tab switching also working.
		if(i == 0) {
			buttonc = addButton(tabsizer, 1, "BUTON C");
		}
	}

	addText(sizer, line++, "Sekme seridini yana kaydirin");

	buttonb = addButton(sizer, line++, "BUTON B");

	// The form block. Each control here now reports its own natural height, so the
	// point of this section is that every one sits at a sensible size and none of
	// them overlaps the row above or below - the same squash test the buttons run,
	// but across controls whose heights differ. The calendar is the tallest and
	// the switch the shortest; if both are whole and clear of their neighbours,
	// the per-control natural heights are being honoured.
	addText(sizer, line++, "-- Form ogeleri --");
	addTextbox(sizer, line++, "Metin kutusu");
	addNumberBox(sizer, line++);
	addSlider(sizer, line++);
	addSwitch(sizer, line++);
	addProgressBar(sizer, line++, gGUIProgressBar::TYPE_LINE);
	addProgressBar(sizer, line++, gGUIProgressBar::TYPE_CIRCULAR);
	addDropdown(sizer, line++, frame);
	addTimebox(sizer, line++);
	addDate(sizer, line++);

	// The squash test. These rows alone need more than the design height, so with
	// a fixed page they would each get about a third of a button and overlap their
	// neighbours. If they are all whole and none of them touches, the page grew.
	for(int i = 0; i < squashbuttoncount; i++) {
		addButton(sizer, line++, "SIKISMA " + std::to_string(i + 1));
	}

	addText(sizer, line++, "ALT - sayfanin sonu");
}

void gScrollTestScenario::onGuiEvent(int guiObjectId, int eventType) {
	if(eventType != G_GUIEVENT_BUTTONRELEASED) return;
	if(buttona && guiObjectId == buttona->getId()) {
		buttonataps++;
		refreshStatus();
	} else if(buttonb && guiObjectId == buttonb->getId()) {
		buttonbtaps++;
		refreshStatus();
	} else if(buttonc && guiObjectId == buttonc->getId()) {
		buttonctaps++;
		refreshStatus();
	}
}
