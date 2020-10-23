/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2014 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_CONTROLPOINT_COLOUR_CONTROL_
#define __PUED_CONTROLPOINT_COLOUR_CONTROL_

#include "ParticleUniverseContextMenu.h"
#include "ParticleUniverseControlPoint.h"

#define ID_COLOUR_CP_CTRL 1
#define ID_ALPHA_CP_CTRL 2

/**	Control that represents a controlpoint, needed in the ControlPointSlider
*/
static const int CONTROL_POINT_CONTROL_RECT_HEIGHT = 48;
static const int CONTROL_POINT_CONTROL_RECT_HALF_HEIGHT = 24;
static const int CONTROL_POINT_CONTROL_Y = CONTROL_POINT_CONTROL_RECT_HEIGHT + 8;
class ControlPointColourControl : public wxControl, ContextMenuCallbackObject
{
	public:
		ControlPointColourControl(std::vector<ControlPoint*>& v,
			wxWindow* parent, 
			wxWindowID id, 
			const wxPoint& pos = wxDefaultPosition, 
			const wxSize& size = wxDefaultSize, 
			const wxString& name = wxT("ControlPointColourControl"),
			float min = 0.0f,
			float max = 1.0f);
		virtual ~ControlPointColourControl(void);
		virtual void callbackContextMenu(wxWindowID id, wxWindow* window);
		void reset(void);
		void addInitialControlpoints(void);
		std::vector<ControlPoint*>& getControlPointList(void);

	protected:
		wxWindow* mParent;
		std::vector<ControlPoint*> mControlPointList;
		ControlPoint* mMovingControlPoint;
		bool mMoving;
		float mMin;
		float mMax;
		ContextMenu* mContextMenu;
		std::vector<ControlPoint*> mInitialControlPointList;

		void OnPaint(wxPaintEvent& event);
		void OnClear(wxEraseEvent& event);
		void OnMouseLButtonPressed(wxMouseEvent& event);
		void OnMouseLButtonReleased(wxMouseEvent& event);
		void OnMove(wxMouseEvent& event);
		void OnMouseRButtonPressed(wxMouseEvent& event);
		void OnWindowEnter(wxMouseEvent& event);
		void fillGradient(wxPaintDC& dc);
		void sortControlPoints(void);
};

#endif