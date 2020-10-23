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

#ifndef __PUED_CONTROLPOINT_DIALOG_H__
#define __PUED_CONTROLPOINT_DIALOG_H__

#include "ParticleUniverseControlPointControl.h"
#include "wx/ogre/prerequisites.h"
#include <wx/dialog.h>
#include "wx/radiobox.h"

#define ID_HORIZONTAL_SLIDER 2010
#define ID_VERTICAL_SLIDER 2011

class ControlPointDialog;

// Sliders to set scale
class HorizontalSlider : public wxSlider
{
	public:
		HorizontalSlider(
		ControlPointDialog* parent,
        wxWindowID id,
        int value,
        int minValue,
        int maxValue,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize);

		/**	Slider moves
		*/
		void OnScroll(wxScrollEvent& evt);

	protected:
		ControlPointDialog* mParent;
};
//-----------------------------------------------------------------------
class VerticalSlider : public wxSlider
{
	public:
		VerticalSlider(
		ControlPointDialog* parent,
        wxWindowID id,
        int value,
        int minValue,
        int maxValue,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize);

		/**	Slider moves
		*/
		void OnScroll(wxScrollEvent& evt);

	protected:
		ControlPointDialog* mParent;
};
//-----------------------------------------------------------------------
// Scrollbars
class HorizontalScrollBar : public wxScrollBar
{
	public:
		HorizontalScrollBar(
		ControlPointDialog* parent,
        wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize);

		/**	Slider moves
		*/
		void OnScroll(wxScrollEvent& evt);

	protected:
		ControlPointDialog* mParent;
};
//-----------------------------------------------------------------------
class VerticalScrollBar : public wxScrollBar
{
	public:
		VerticalScrollBar(
		ControlPointDialog* parent,
        wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize);

		/**	Slider moves
		*/
		void OnScroll(wxScrollEvent& evt);

	protected:
		ControlPointDialog* mParent;
};
//-----------------------------------------------------------------------
class InterpolationRadioBox : public wxRadioBox
{
	public:
		InterpolationRadioBox(ControlPointDialog* parent, wxWindowID id, const wxArrayString& choices, int dim);
		virtual ~InterpolationRadioBox(void){};
		void OnInterpolationType(wxCommandEvent& event);
		void setInterpolationType(ParticleUniverse::InterpolationType type);

	protected:
		ControlPointDialog* mParent;
};
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
/**	Adding controlpoints
*/
class ControlPointDialog : public wxDialog
{
	public:
		// Constructor / Destructor
		ControlPointDialog(
			ParticleUniverse::InterpolationType interpolationType,
			const ParticleUniverse::DynamicAttributeCurved::ControlPointList controlPointList,
			wxWindow* parent, 
			wxWindowID id, 
			const wxString& title, 
			const wxPoint& pos = wxDefaultPosition, 
			const wxSize& size = wxDefaultSize, 
			const wxString& name = wxT("ControlPointDialog"));
		virtual ~ControlPointDialog(void){};
		void OnReset(wxCommandEvent& event);
		bool Destroy();
		void setHorizontalScale(int horizontal);
		void setVerticalScale(int vertical);
		void setHorizontalScroll(int horizontal);
		void setVerticalScroll(int vertical);
		const ParticleUniverse::DynamicAttributeCurved::ControlPointList getControlPointList(void);
		const ParticleUniverse::InterpolationType getInterpolationType(void);
		void setInterpolationType(ParticleUniverse::InterpolationType type);


	protected:
		ControlPointControl* mControlPointControl; // The wxWidgets control with which control points can be editted
		std::vector<ControlPoint*> mControlPoints; // The controlpoints of the wxWidgets control
		ParticleUniverse::DynamicAttributeCurved::ControlPointList mControlPointsForPUCommunication; // The controlpoints received from the PU object
		HorizontalSlider* mHorizontalSlider;
		VerticalSlider* mVerticalSlider;
		HorizontalScrollBar* mHorizontalScrollBar;
		VerticalScrollBar* mVerticalScrollBar;
		int mPreviousHorizontalScale;
		int mPreviousVerticalScale;
		int mPreviousHorizontalScroll;
		int mPreviousVerticalScroll;
		InterpolationRadioBox* mRadioBox;
};

#endif
