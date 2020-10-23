/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/ColourAffectorDialog.h"
#include <wx/wx.h>

//-----------------------------------------------------------------------
ColourAffectorDialog::ColourAffectorDialog(ParticleUniverse::ColourAffector::ColourMap& map,
	wxWindow* parent, 
	wxWindowID id, 
	const wxString& title, 
	const wxPoint& pos, 
	const wxSize& size, 
	const wxString& name) : 
wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP, name),
mColourMap(map)
{
	mControlSize = wxSize(480, 96);
	ControlPoint* cp;
	ParticleUniverse::ColourAffector::ColourMap::iterator itColourMap;
	ParticleUniverse::ColourAffector::ColourMap::iterator itColourMapEnd = mColourMap.end();
	float time;
	ParticleUniverse::ColourValue colour;
	for (itColourMap = mColourMap.begin(); itColourMap != itColourMapEnd; ++itColourMap)
	{
		time = itColourMap->first;
		colour = itColourMap->second;
		cp = new ControlPoint(ControlPoint::CTRL_POINT_TYPE_SOLID, wxPoint(time * (mControlSize.x - CONTROL_POINT_RECT_SIZE - CONTROL_POINT_BORDER_SIZE_BOLD), CONTROL_POINT_CONTROL_Y), true);
		wxColour col(255*colour.r, 255*colour.g, 255*colour.b, 255*colour.a);
		cp->setColour(col);
		mControlPoints.push_back(cp);
	}

	wxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* txtHeaderText = new wxStaticText(this, -1, _("0%                                                          % Time to live ->                                                          100%"));
	headerSizer->Add(txtHeaderText, 0, wxALIGN_CENTER_HORIZONTAL, 0);

	// Use one panel per control, because the control uses the wxPaintDC of the whole panel
	wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(560, 80), wxBORDER_NONE);
	mControlPointColourControl = new ControlPointColourControl(mControlPoints, panel, wxID_ANY, wxDefaultPosition, mControlSize);

	wxSizer* oKCancelSizer = CreateButtonSizer(wxOK | wxCANCEL);
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* buttonReset = new wxButton(this, ID_RESET, _("Reset"));
	buttonSizer->Add(buttonReset, 0, wxALL|wxALIGN_RIGHT, 0);
	buttonSizer->AddSpacer(240);
	buttonSizer->Add(oKCancelSizer, 0, wxALL|wxALIGN_LEFT, 0);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(headerSizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 8);
	mainSizer->Add(panel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 0);
	mainSizer->Add(buttonSizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 24);
	SetSizer(mainSizer);
	mainSizer->Layout();

	Connect(ID_RESET, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ColourAffectorDialog::OnReset));
}
//-----------------------------------------------------------------------
void ColourAffectorDialog::OnReset(wxCommandEvent& event)
{
	mControlPointColourControl->reset();
}
//-----------------------------------------------------------------------
bool ColourAffectorDialog::Destroy()
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
ParticleUniverse::ColourAffector::ColourMap& ColourAffectorDialog::getColourMap(void)
{
	std::vector<ControlPoint*> controlPoints = mControlPointColourControl->getControlPointList();
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = controlPoints.end();
	ControlPoint* cp;
	float time;
	mColourMap.clear();
	for (it = controlPoints.begin(); it != itEnd; ++it)
	{
		cp = *it;
		time = (float)cp->getPosition().x / (float)(mControlSize.x - CONTROL_POINT_RECT_SIZE - CONTROL_POINT_BORDER_SIZE_BOLD);
		wxColour col = cp->getColour();
		ParticleUniverse::ColourValue colour(col.Red()/255.0f, col.Green()/255.0f, col.Blue()/255.0f, col.Alpha()/255.0f);
		mColourMap.insert(std::make_pair(time, colour));
	}
	return mColourMap;
}
