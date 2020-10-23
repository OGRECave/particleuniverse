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
#include "ParticleUniverseSliderDialog.h"
#include <wx/wx.h>

//-----------------------------------------------------------------------
SliderDialog::SliderDialog(wxWindow* parent, 
	wxWindowID id, 
	const wxString& title, 
	const wxPoint& pos, 
	const wxSize& size, 
	const wxString& name,
	float value,
	float min,
	float max,
	float divider) : 
	wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP, name),
	mDivider(divider),
	mValue(value/divider)
{
	mSlider = new wxSlider(this, ID_SLIDER, max, min, max, wxDefaultPosition, wxSize(0.5 * size.x, 32));
	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* oKSizer = CreateButtonSizer(wxOK);
	Ogre::String s = Ogre::StringConverter::toString(mValue, 2);
	mValueCtrl = new wxTextCtrl(this, ID_TEXT_ENTERED, ogre2wx(s), wxDefaultPosition, wxSize(40, 24));
	mSlider->SetValue(value);

	mainSizer->AddSpacer(16);
	mainSizer->Add(mValueCtrl, 0, wxALL, 8);
	mainSizer->Add(mSlider, 0, wxALL, 8);
	mainSizer->AddSpacer(8);
	mainSizer->Add(oKSizer, 0, wxALL, 8);
	SetSizer(mainSizer);
	mainSizer->Layout();

	Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(SliderDialog::OnTextUpdated));
	Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(SliderDialog::OnScroll));
	Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(SliderDialog::OnScroll));
}
//-----------------------------------------------------------------------
void SliderDialog::OnTextUpdated(wxCommandEvent& evt)
{
	Ogre::String s = wx2ogre(mValueCtrl->GetLineText(0));
	mValue = Ogre::StringConverter::parseReal(s);
	mSlider->SetValue(mValue * mDivider);
}
//-----------------------------------------------------------------------
void SliderDialog::OnScroll(wxScrollEvent& evt)
{
	if (!mSlider || mDivider == 0)
		return;

	mValue = (float)mSlider->GetValue() / mDivider;
	Ogre::String s = Ogre::StringConverter::toString(mValue, 2);
	mValueCtrl->SetValue(ogre2wx(s));
}
//-----------------------------------------------------------------------
float SliderDialog::getValue(void) const
{
	return mValue;
}
