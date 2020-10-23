/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_SLIDER_DIALOG_H__
#define __PUED_SLIDER_DIALOG_H__

#include "ParticleUniverseSliderDialog.h"
#include <wx/dialog.h>

#define ID_TEXT_ENTERED 2000
#define ID_SLIDER 2001

class SliderDialog : public wxDialog
{
	public:
		// Constructor / Destructor
		SliderDialog(wxWindow* parent, 
			wxWindowID id, 
			const wxString& title, 
			const wxPoint& pos = wxDefaultPosition, 
			const wxSize& size = wxDefaultSize, 
			const wxString& name = wxT("SliderDialog"),
			float value = 0.0f,
			float min = 0.0f,
			float max = 255.0f,
			float divider = 255.0f);
		~SliderDialog(void){};
		void OnTextUpdated(wxCommandEvent& evt);
		void OnScroll(wxScrollEvent& evt);
		float getValue(void) const;

	protected:
		float mDivider;
		wxTextCtrl* mValueCtrl;
		wxSlider* mSlider;
		float mValue;
};

#endif
