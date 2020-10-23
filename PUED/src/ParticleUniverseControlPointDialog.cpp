/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseControlPointDialog.h"
#include <wx/wx.h>

//-----------------------------------------------------------------------
HorizontalSlider::HorizontalSlider(
		ControlPointDialog* parent,
        wxWindowID id,
        int value,
        int minValue,
        int maxValue,
        const wxPoint& pos,
        const wxSize& size) : 
	wxSlider (parent, id, value, minValue, maxValue, pos, size, wxSL_HORIZONTAL),
		mParent(parent)
{
	Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(HorizontalSlider::OnScroll));
	Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(HorizontalSlider::OnScroll));
}
//-----------------------------------------------------------------------
void HorizontalSlider::OnScroll(wxScrollEvent& evt)
{
	mParent->setHorizontalScale(GetValue());
}
//-----------------------------------------------------------------------
VerticalSlider::VerticalSlider(
		ControlPointDialog* parent,
        wxWindowID id,
        int value,
        int minValue,
        int maxValue,
        const wxPoint& pos,
        const wxSize& size) : 
	wxSlider (parent, id, value, minValue, maxValue, pos, size, wxSL_VERTICAL | wxSL_INVERSE),
		mParent(parent)
{
	Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(VerticalSlider::OnScroll));
	Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(VerticalSlider::OnScroll));
}
//-----------------------------------------------------------------------
void VerticalSlider::OnScroll(wxScrollEvent& evt)
{
	mParent->setVerticalScale(GetValue());
}
//-----------------------------------------------------------------------
HorizontalScrollBar::HorizontalScrollBar(
		ControlPointDialog* parent,
        wxWindowID id,
        const wxPoint& pos,
        const wxSize& size) : 
	wxScrollBar(parent, id, pos, size, wxHORIZONTAL),
	mParent(parent)
{
	Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(HorizontalScrollBar::OnScroll));
	Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(HorizontalScrollBar::OnScroll));
}
//-----------------------------------------------------------------------
void HorizontalScrollBar::OnScroll(wxScrollEvent& evt)
{
	mParent->setHorizontalScroll(GetThumbPosition());
}
//-----------------------------------------------------------------------
VerticalScrollBar::VerticalScrollBar(
		ControlPointDialog* parent,
        wxWindowID id,
        const wxPoint& pos,
        const wxSize& size) : 
	wxScrollBar(parent, id, pos, size, wxVERTICAL),
	mParent(parent)
{
	Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(VerticalScrollBar::OnScroll));
	Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(VerticalScrollBar::OnScroll));
}
//-----------------------------------------------------------------------
void VerticalScrollBar::OnScroll(wxScrollEvent& evt)
{
	mParent->setVerticalScroll(GetThumbPosition());
}
//-----------------------------------------------------------------------
InterpolationRadioBox::InterpolationRadioBox(ControlPointDialog* parent, wxWindowID id, const wxArrayString& choices, int dim) :
	wxRadioBox(parent, id, _("Interpolation"), wxDefaultPosition, wxDefaultSize, choices, dim, wxRA_SPECIFY_COLS),
	mParent(parent)
{
	Connect(wxID_ANY, wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(InterpolationRadioBox::OnInterpolationType));
}
//-----------------------------------------------------------------------
void InterpolationRadioBox::OnInterpolationType(wxCommandEvent& event)
{
	if (GetSelection() == 0)
	{
		mParent->setInterpolationType(ParticleUniverse::IT_LINEAR);
	}
	else
	{
		mParent->setInterpolationType(ParticleUniverse::IT_SPLINE);
	}
}
//-----------------------------------------------------------------------
void InterpolationRadioBox::setInterpolationType(ParticleUniverse::InterpolationType type)
{
	if (type == ParticleUniverse::IT_LINEAR)
	{
		SetSelection(0);
	}
	else
	{
		SetSelection(1);
	}
}
//-----------------------------------------------------------------------
ControlPointDialog::ControlPointDialog(
	ParticleUniverse::InterpolationType interpolationType,
	const ParticleUniverse::DynamicAttributeCurved::ControlPointList controlPointList,
	wxWindow* parent, 
	wxWindowID id, 
	const wxString& title, 
	const wxPoint& pos, 
	const wxSize& size, 
	const wxString& name) : 
wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP, name),
mPreviousHorizontalScale(1.0f),
mPreviousVerticalScale(1.0f),
mPreviousHorizontalScroll(5.0f),
mPreviousVerticalScroll(5.0f),
mControlPointsForPUCommunication(controlPointList)
{
	// Fill mControlPoints with values passed in the dialog
	ControlPoint* cp;
	ParticleUniverse::DynamicAttributeCurved::ControlPointList::const_iterator it;
	ParticleUniverse::DynamicAttributeCurved::ControlPointList::const_iterator itEnd = mControlPointsForPUCommunication.end();
	for (it = mControlPointsForPUCommunication.begin(); it != itEnd; ++it)
	{
		cp = new ControlPoint(ControlPoint::CTRL_POINT_TYPE_GRID, wxDefaultPosition, true);
		cp->setXval((*it).x);
		cp->setYval((*it).y);
		mControlPoints.push_back(cp);
	}

	// Setup dialog
	wxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);
	wxSize controlSize = wxSize(600, 400);

	// Sliders
	mHorizontalSlider = new HorizontalSlider(this, ID_HORIZONTAL_SLIDER, mPreviousHorizontalScale, 1, 5, wxDefaultPosition, wxSize(controlSize.x, 32));
	mVerticalSlider = new VerticalSlider(this, ID_VERTICAL_SLIDER, mPreviousVerticalScale, 1, 8, wxDefaultPosition, wxSize(32, controlSize.y));
	headerSizer->Add(mHorizontalSlider, 0, wxALIGN_CENTER_HORIZONTAL, 0);

	// Use one panel per control, because the control uses the wxPaintDC of the whole panel
	wxSizer* midSizer = new wxBoxSizer(wxHORIZONTAL);
	wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, controlSize, wxBORDER_NONE);
	mVerticalScrollBar = new VerticalScrollBar(this, wxID_ANY, wxDefaultPosition, wxSize(16, controlSize.y));
	mVerticalScrollBar->SetScrollbar(mPreviousVerticalScroll, 1, 11, 1, true); 
	midSizer->Add(mVerticalSlider, 0, wxALIGN_CENTER_HORIZONTAL, 0);
	midSizer->Add(panel, 0, wxALIGN_CENTER_HORIZONTAL, 0);
	midSizer->Add(mVerticalScrollBar, 0, wxALIGN_CENTER_HORIZONTAL, 0);
	mControlPointControl = new ControlPointControl(mControlPoints, panel, wxID_ANY, wxDefaultPosition, controlSize);

	// Radiobox
	wxArrayString s;
	s.Add(_("&Linear interpolation"));
	s.Add(_("&Spline interpolation"));
	wxBoxSizer* radioBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	mRadioBox = new InterpolationRadioBox(this, wxID_ANY, s, 2);
	radioBoxSizer->Add(110, 0);
	radioBoxSizer->Add(mRadioBox, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0);

	// Buttons
	wxSizer* oKCancelSizer = CreateButtonSizer(wxOK | wxCANCEL);
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* buttonReset = new wxButton(this, ID_RESET, _("Reset"));
	buttonSizer->Add(buttonReset, 0, wxALL|wxALIGN_RIGHT, 0);
	buttonSizer->AddSpacer(320);
	buttonSizer->Add(oKCancelSizer, 0, wxALL|wxALIGN_LEFT, 0);

	// Set the interpolation type here
	setInterpolationType(interpolationType);

	// Hotizontal scrollbar
	wxBoxSizer* horizontalScrollBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	mHorizontalScrollBar = new HorizontalScrollBar(this, wxID_ANY, wxDefaultPosition, wxSize(controlSize.x, 16));
	mHorizontalScrollBar->SetScrollbar(mPreviousHorizontalScroll, 1, 11, 1, true); 
	horizontalScrollBoxSizer->Add(16, 0);
	horizontalScrollBoxSizer->Add(mHorizontalScrollBar, 0, wxALIGN_CENTER_HORIZONTAL, 0);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(headerSizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 8);
	mainSizer->Add(midSizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0);
	mainSizer->Add(horizontalScrollBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL, 0);

	mainSizer->Add(radioBoxSizer, 0, wxALL|wxALIGN_LEFT, 8);
	mainSizer->Add(buttonSizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 16);
	SetSizer(mainSizer);
	mainSizer->Layout();

	Connect(ID_RESET, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPointDialog::OnReset));
}
//-----------------------------------------------------------------------
void ControlPointDialog::OnReset(wxCommandEvent& event)
{
	mControlPointControl->reset();
}
//-----------------------------------------------------------------------
bool ControlPointDialog::Destroy()
{
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mControlPoints.end();
	for (it = mControlPoints.begin(); it != itEnd; ++it)
	{
		delete (*it);
	}
	mControlPoints.clear();
	return wxDialog::Destroy();
}
//-----------------------------------------------------------------------
void ControlPointDialog::setHorizontalScale(int horizontal)
{
	if (!mControlPointControl)
		return;

	if (!horizontal)
		return;

	float scale = 1.0f;
	if (mPreviousHorizontalScale > horizontal)
	{
		scale = 0.5f;
	}
	else if (mPreviousHorizontalScale < horizontal)
	{
		scale = 2.0f;
	}

	mControlPointControl->scaleMinMaxXGraph(scale);
	mPreviousHorizontalScale = horizontal;
	Refresh();
}
//-----------------------------------------------------------------------
void ControlPointDialog::setVerticalScale(int vertical)
{
	if (!mControlPointControl)
		return;

	if (!vertical)
		return;

	float scale = 1.0f;
	if (mPreviousVerticalScale > vertical)
	{
		scale = 0.5f;
	}
	else if (mPreviousVerticalScale < vertical)
	{
		scale = 2.0f;
	}

	mControlPointControl->scaleMinMaxYGraph(scale);
	mPreviousVerticalScale = vertical;
	Refresh();
}
//-----------------------------------------------------------------------
void ControlPointDialog::setHorizontalScroll(int horizontal)
{
	if (!mControlPointControl)
		return;

	if (mPreviousHorizontalScroll == horizontal)
		return;

	float scroll = 1.0f;
	if (mPreviousHorizontalScroll < horizontal)
	{
		scroll = -1.0f;
	}

	mControlPointControl->scrollMinMaxXGraph(scroll);
	mPreviousHorizontalScroll = horizontal;
	Refresh();
}
//-----------------------------------------------------------------------
void ControlPointDialog::setVerticalScroll(int vertical)
{
	if (!mControlPointControl)
		return;

	if (mPreviousVerticalScroll == vertical)
		return;

	float scroll = 1.0f;
	if (mPreviousVerticalScroll > vertical)
	{
		scroll = -1.0f;
	}

	mControlPointControl->scrollMinMaxYGraph(scroll);
	mPreviousVerticalScroll = vertical;
	Refresh();
}
//-----------------------------------------------------------------------
const ParticleUniverse::DynamicAttributeCurved::ControlPointList ControlPointDialog::getControlPointList(void)
{
	std::vector<ControlPoint*> controlPoints = mControlPointControl->getControlPointList();
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = controlPoints.end();
	ControlPoint* cp;
	mControlPointsForPUCommunication.clear();
	for (it = controlPoints.begin(); it != itEnd; ++it)
	{
		cp = *it;
		Ogre::Vector2 v;
		v.x = cp->getXval();
		v.y = cp->getYval();
		mControlPointsForPUCommunication.push_back(v);
	}
	return mControlPointsForPUCommunication;
}
//-----------------------------------------------------------------------
const ParticleUniverse::InterpolationType ControlPointDialog::getInterpolationType(void)
{
	if (mControlPointControl && mControlPointControl->useLinearInterpolation())
	{
		return ParticleUniverse::IT_LINEAR;
	}
	else
	{
		return ParticleUniverse::IT_SPLINE;
	}
}
//-----------------------------------------------------------------------
void ControlPointDialog::setInterpolationType(ParticleUniverse::InterpolationType type)
{
	if (!mControlPointControl)
		return;

	if (type == ParticleUniverse::IT_LINEAR)
	{
		mControlPointControl->setLinearInterpolation(true);
	}
	else
	{
		mControlPointControl->setLinearInterpolation(false);
	}
	if (mRadioBox)
	{
		mRadioBox->setInterpolationType(type);
	}
}
