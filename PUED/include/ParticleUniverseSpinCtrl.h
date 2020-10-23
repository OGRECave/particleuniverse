/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_SPIN_CTRL_H__
#define __PUED_SPIN_CTRL_H__

#include "wx/ogre/prerequisites.h"
#include "wx/spinctrl.h"

/**	Spin control for X, Y, Z axis
*/
class UIMiscIcons;
class SpinCtrlXYZ : public wxSpinCtrl
{
	public:

		SpinCtrlXYZ (UIMiscIcons* icons, 
			wxWindow* parent,
			wxWindowID id = -1, 
			const wxString& value = wxT(""), 
			const wxPoint& pos = wxDefaultPosition, 
			const wxSize& size = wxDefaultSize, 
			long style = wxSP_ARROW_KEYS, 
			int min = 0, 
			int max = 100, 
			int initial = 0, 
			const wxString& name = _T("wxSpinCtrl"),
			SpinCtrlXYZ* nextSpinCtrl = 0);

		virtual ~SpinCtrlXYZ(void){};
		void setNextSpinCtrl(SpinCtrlXYZ* nextSpinCtrl);
		void OnSpinUpdate(wxCommandEvent& event);
		virtual void OnChar(wxKeyEvent& event);

	protected:
		UIMiscIcons* mUIMiscIcons;
		SpinCtrlXYZ* mNextSpinCtrl;
};

#endif
