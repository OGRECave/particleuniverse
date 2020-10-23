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

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseSpinCtrl.h"
#include "ParticleUniverseMainToolbar.h"

//-----------------------------------------------------------------------
SpinCtrlXYZ::SpinCtrlXYZ (UIMiscIcons* icons, 
	wxWindow* parent,
	wxWindowID id, 
	const wxString& value, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	int min, 
	int max, 
	int initial,
	const wxString& name,
	SpinCtrlXYZ* nextSpinCtrl) : 
	wxSpinCtrl(parent, id, value, pos, size, style, min, max, initial, name), 
	mUIMiscIcons(icons),
	mNextSpinCtrl(nextSpinCtrl)
{
	Connect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler(SpinCtrlXYZ::OnSpinUpdate));
	Connect(wxEVT_COMMAND_TEXT_ENTER, wxKeyEventHandler(SpinCtrlXYZ::OnChar));
	Connect(wxEVT_CHAR, wxKeyEventHandler(SpinCtrlXYZ::OnChar));
}
//-----------------------------------------------------------------------
void SpinCtrlXYZ::setNextSpinCtrl(SpinCtrlXYZ* nextSpinCtrl)
{
	mNextSpinCtrl = nextSpinCtrl;
}
//-----------------------------------------------------------------------
void SpinCtrlXYZ::OnSpinUpdate(wxCommandEvent& event)
{
	mUIMiscIcons->callbackSpinCtrlXYZ(this);
}
//-----------------------------------------------------------------------
void SpinCtrlXYZ::OnChar(wxKeyEvent& event)
{
    switch ( event.GetKeyCode() )
    {
		case WXK_RETURN:
		{
			mUIMiscIcons->callbackSpinCtrlXYZ(this);
			if (mNextSpinCtrl)
			{
				mNextSpinCtrl->SetFocus();
				mNextSpinCtrl->SetSelection(0, 3);
			}
		}

        case WXK_TAB:
		{
			mUIMiscIcons->callbackSpinCtrlXYZ(this);
			if (mNextSpinCtrl)
			{
				mNextSpinCtrl->SetFocus();
				mNextSpinCtrl->SetSelection(0, 3);
			}
		}
		break;
    }
}