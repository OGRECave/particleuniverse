/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseControlPointColourControl.h"
#include "ParticleUniverseSliderDialog.h"
#include <wx/colordlg.h>

//-----------------------------------------------------------------------
ControlPointColourControl::ControlPointColourControl(std::vector<ControlPoint*>& v,
			wxWindow* parent, 
			wxWindowID id, 
			const wxPoint& pos, 
			const wxSize& size, 
			const wxString& name,
			const float min,
			const float max) : 
	wxControl (parent, id, pos, size, 0, wxDefaultValidator, name),
	ContextMenuCallbackObject(),
	mParent(parent),
	mMovingControlPoint(0),
	mMoving(false),
	mMin(min),
	mMax(max),
	mContextMenu(0),
	mInitialControlPointList(v)
{
	// Setup context menu
	mContextMenu = new ContextMenu(this, this, wxDefaultPosition, wxDefaultSize);
	mContextMenu->addMenuElement(ID_COLOUR_CP_CTRL, 
		_("        Select colour"), 
		wxT("change_colour.png"),
		wxT("change_colour.png"),
		wxSize(120, 16));
	mContextMenu->addMenuElement(ID_ALPHA_CP_CTRL, 
		_("        Set alpha"), 
		wxT("grid_icon.png"),
		wxT("grid_icon.png"),
		wxSize(120, 16));

	mContextMenu->enableMenuElement(ID_COLOUR_CP_CTRL, true);
	mContextMenu->enableMenuElement(ID_ALPHA_CP_CTRL, true);
	mContextMenu->initialise();

	// Fill the controlpoints
	reset();

	// Connect the events
	Connect(wxEVT_PAINT, wxPaintEventHandler(ControlPointColourControl::OnPaint));
	Connect(wxEVT_ERASE_BACKGROUND , wxEraseEventHandler(ControlPointColourControl::OnClear));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ControlPointColourControl::OnMouseLButtonPressed));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ControlPointColourControl::OnMouseLButtonReleased));
	Connect(wxEVT_MOTION, wxMouseEventHandler(ControlPointColourControl::OnMove));
	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(ControlPointColourControl::OnMouseRButtonPressed));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(ControlPointColourControl::OnWindowEnter));
}
//-----------------------------------------------------------------------
ControlPointColourControl::~ControlPointColourControl(void)
{
	delete mContextMenu;

	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
	for (it = mControlPointList.begin(); it != itEnd; ++it)
	{
		delete *it;
	}
	mControlPointList.clear();
}
//-----------------------------------------------------------------------
void ControlPointColourControl::OnPaint(wxPaintEvent& event)
{
	SetPosition(wxPoint(0, 0)); // Always set the canvas to the original position
	wxPaintDC dc(this);

	// Draw all control points
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
	for (it = mControlPointList.begin(); it != itEnd; ++it)
	{
		(*it)->paint(dc);
	}

	// Fill the gradient
	fillGradient(dc);
}
//-----------------------------------------------------------------------
void ControlPointColourControl::OnClear(wxEraseEvent& event)
{
	wxDC* dc = event.GetDC();
	wxColour col = GetBackgroundColour();
	dc->SetPen(wxPen(col, CONTROL_POINT_BORDER_SIZE, wxSOLID));
	dc->SetBrush(wxBrush(col, wxSOLID));
	dc->DrawRectangle(0, CONTROL_POINT_CONTROL_RECT_HEIGHT, GetSize().x, 800);
}
//------------------------------------------------------------------------------
void ControlPointColourControl::OnMouseLButtonPressed(wxMouseEvent& event)
{
	// First check if one of the controlpoints was hit. If is this the case, the controlpoint has state 'selected'.
	mMovingControlPoint = 0;
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();

	for (it = mControlPointList.begin(); it != itEnd; ++it)
	{
		wxPoint cpPos = (*it)->getPosition();
		if (event.GetPosition().x > cpPos.x &&
				event.GetPosition().y > cpPos.y &&
				event.GetPosition().x < cpPos.x + CONTROL_POINT_RECT_SIZE &&
				event.GetPosition().y < cpPos.y + CONTROL_POINT_RECT_SIZE)
		{
			// If non-movable controlpoints are selected, continue with searching
			if ((*it)->isMovable())
			{
				mMovingControlPoint = *it;
				mMoving = true; // Must be set to true so the mouse move event can be handled correctly (prevent that cp sticks to the mouse)
				Refresh();
				return;
			}
		}
	}

	// Check if one of the controlpoints is deleted
	for (it = mControlPointList.begin(); it != itEnd; ++it)
	{
		wxPoint cpPos = (*it)->getPosition();
		if (event.GetPosition().x > cpPos.x &&
				event.GetPosition().y > cpPos.y + CONTROL_POINT_RECT_SIZE &&
				event.GetPosition().x < cpPos.x + CONTROL_POINT_RECT_SIZE &&
				event.GetPosition().y < cpPos.y + 2 * CONTROL_POINT_RECT_SIZE)
		{
			// Only delete the controlpoint that can be moved
			if ((*it)->isMovable())
			{
				delete *it;
				mControlPointList.erase(it);
				Refresh();
				return;
			}
		}
	}

	// Check whether the click was in the 'zone'
//	if (event.GetPosition().y < CONTROL_POINT_CONTROL_RECT_HEIGHT || 
//		event.GetPosition().x < CONTROL_POINT_RECT_SIZE || 
//		event.GetPosition().x > GetSize().x - CONTROL_POINT_RECT_SIZE - CONTROL_POINT_BORDER_SIZE_BOLD)
//		return;
	if (event.GetPosition().x < CONTROL_POINT_RECT_SIZE || 
		event.GetPosition().x > GetSize().x - CONTROL_POINT_RECT_SIZE - CONTROL_POINT_BORDER_SIZE_BOLD)
		return;

	// Create a new control point
	ControlPoint* cp = new ControlPoint(
		ControlPoint::CTRL_POINT_TYPE_SOLID,
		wxPoint(event.GetPosition().x, CONTROL_POINT_CONTROL_Y), 
		true,
		GetSize().x);
	mControlPointList.push_back(cp);
	mMovingControlPoint = cp;
	mMoving = true;
	Refresh();
}
//------------------------------------------------------------------------------
void ControlPointColourControl::OnMouseLButtonReleased(wxMouseEvent& event)
{
	mMoving = false;
	if (mMovingControlPoint)
	{
		mMovingControlPoint->setMoving(false);
		Refresh();
	}
	mMovingControlPoint = 0;
}
//------------------------------------------------------------------------------
void ControlPointColourControl::OnMove(wxMouseEvent& event)
{
	if (!mMoving)
		return;

	// Moving is only allowed in the 'zone'
	if (event.GetPosition().x < 0 || event.GetPosition().x > GetSize().x - CONTROL_POINT_RECT_SIZE - CONTROL_POINT_BORDER_SIZE_BOLD)
		return;

	if (mMovingControlPoint && mMovingControlPoint->isMovable())
	{
		mMoving = true;
		mMovingControlPoint->setMoving(true);
		Freeze();
		mMovingControlPoint->setPosition(wxPoint(event.GetPosition().x, CONTROL_POINT_CONTROL_Y));
		Refresh();
		Thaw();
	}
}
//------------------------------------------------------------------------------
void ControlPointColourControl::OnMouseRButtonPressed(wxMouseEvent& event)
{
	// First check if one of the controlpoints was hit. If is this the case, decide what happens
	mMovingControlPoint = 0;
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
	for (it = mControlPointList.begin(); it != itEnd; ++it)
	{
		wxPoint cpPos = (*it)->getPosition();
		if (event.GetPosition().x > cpPos.x &&
				event.GetPosition().y > cpPos.y &&
				event.GetPosition().x < cpPos.x + CONTROL_POINT_RECT_SIZE &&
				event.GetPosition().y < cpPos.y + CONTROL_POINT_RECT_SIZE)
		{
			mMovingControlPoint = *it;
			mContextMenu->showContextMenu(GetGrandParent()->GetPosition() + event.GetPosition());
			return;
		}
	}
}
//------------------------------------------------------------------------------
void ControlPointColourControl::OnWindowEnter(wxMouseEvent& event)
{
	if (!event.ButtonIsDown(wxMOUSE_BTN_LEFT))
	{
		mMoving = false;
		if (mMovingControlPoint)
		{
			mMovingControlPoint->setMoving(false);
			Refresh();
		}
	}
}
//------------------------------------------------------------------------------
void ControlPointColourControl::fillGradient(wxPaintDC& dc)
{
	if (mControlPointList.size() < 2)
		return;

	sortControlPoints();
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
	it = mControlPointList.begin();
	wxColour firstColour = (*it)->getColour();
	wxPoint firstPosition = (*it)->getPosition();
	wxColour col;
	wxColour nextColour;
	wxPoint nextPosition;
	float diff;
	float r, r1, r2;
	float g, g1, g2;
	float b, b1, b2;
	float a, a1, a2;
	it++;

	// Because wxWidgets does not allow alpha in draw functions, we have to fake it.
	while (it != itEnd)
	{
		nextColour = (*it)->getColour();
		nextPosition = (*it)->getPosition();
		diff = nextPosition.x - firstPosition.x;
		r = (float)((nextColour.Red() - firstColour.Red())) / diff;
		g = (float)((nextColour.Green() - firstColour.Green())) / diff;
		b = (float)((nextColour.Blue() - firstColour.Blue())) / diff;
		a = (float)((nextColour.Alpha() - firstColour.Alpha())) / diff;

		int count = 0;
		for (int i = firstPosition.x; i < nextPosition.x; ++i)
		{
			int time = i + i * CONTROL_POINT_RECT_SIZE/GetSize().x;
			if ((time / CONTROL_POINT_CONTROL_RECT_HALF_HEIGHT) % 2 == 0)
			{
				// Make alpha pattern 1 0 1 0
				// Upper part
				r1 = firstColour.Red() + count * r;
				g1 = firstColour.Green() + count * g;
				b1 = firstColour.Blue() + count * b;
				a1 = firstColour.Alpha() + count * a;

				r1 = (a1 * r1 + (255 - a1) * 200) / 255;
				g1 = (a1 * g1 + (255 - a1) * 200) / 255;
				b1 = (a1 * b1 + (255 - a1) * 200) / 255;
				col = wxColour(r1, g1, b1);
				dc.SetPen(wxPen(col, 2, wxSOLID));
				dc.DrawLine(time, 0, time, CONTROL_POINT_CONTROL_RECT_HALF_HEIGHT);

				// Lower part
				r2 = firstColour.Red() + count * r;
				g2 = firstColour.Green() + count * g;
				b2 = firstColour.Blue() + count * b;
				a2 = firstColour.Alpha() + count * a;

				r2 = (a2 * r2 + (255 - a2) * 40) / 255;
				g2 = (a2 * g2 + (255 - a2) * 40) / 255;
				b2 = (a2 * b2 + (255 - a2) * 40) / 255;
				col = wxColour(r2, g2, b2);
				dc.SetPen(wxPen(col, 2, wxSOLID));
				dc.DrawLine(time, CONTROL_POINT_CONTROL_RECT_HALF_HEIGHT, time, CONTROL_POINT_CONTROL_RECT_HEIGHT);
			}
			else
			{
				// Make alpha pattern 0 1 0 1
				// Upper part
				r1 = firstColour.Red() + count * r;
				g1 = firstColour.Green() + count * g;
				b1 = firstColour.Blue() + count * b;
				a1 = firstColour.Alpha() + count * a;

				r1 = (a1 * r1 + (255 - a1) * 40) / 255;
				g1 = (a1 * g1 + (255 - a1) * 40) / 255;
				b1 = (a1 * b1 + (255 - a1) * 40) / 255;
				col = wxColour(r1, g1, b1);
				dc.SetPen(wxPen(col, 2, wxSOLID));
				dc.DrawLine(time, 0, time, CONTROL_POINT_CONTROL_RECT_HALF_HEIGHT);

				// Lower part
				r2 = firstColour.Red() + count * r;
				g2 = firstColour.Green() + count * g;
				b2 = firstColour.Blue() + count * b;
				a2 = firstColour.Alpha() + count * a;

				r2 = (a2 * r2 + (255 - a2) * 200) / 255;
				g2 = (a2 * g2 + (255 - a2) * 200) / 255;
				b2 = (a2 * b2 + (255 - a2) * 200) / 255;
				col = wxColour(r2, g2, b2);
				dc.SetPen(wxPen(col, 2, wxSOLID));
				dc.DrawLine(time, CONTROL_POINT_CONTROL_RECT_HALF_HEIGHT, time, CONTROL_POINT_CONTROL_RECT_HEIGHT);
			}

			++count;
		}
		firstColour = (*it)->getColour();
		firstPosition = nextPosition;
		++it;
	}
}
//------------------------------------------------------------------------------
void ControlPointColourControl::sortControlPoints(void)
{
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
	std::multimap<int, ControlPoint*> cpMap;
	for (it = mControlPointList.begin(); it != itEnd; ++it)
	{
		cpMap.insert(std::make_pair((*it)->getPosition().x, *it));
	}
	mControlPointList.clear();
	std::multimap<int, ControlPoint*>::iterator itMap;
	std::multimap<int, ControlPoint*>::iterator itMapEnd = cpMap.end();
	for (itMap = cpMap.begin(); itMap != itMapEnd; ++itMap)
	{
		mControlPointList.push_back(itMap->second);
	}
}
//-----------------------------------------------------------------------
void ControlPointColourControl::callbackContextMenu(wxWindowID id, wxWindow* window)
{
	if (mContextMenu)
	{
		mContextMenu->hideContextMenu();
	}

	switch (id)
	{
		case ID_COLOUR_CP_CTRL:
			{
				if (mMovingControlPoint)
				{
					wxColourData colourData;
					colourData.SetColour(mMovingControlPoint->getColour());
					wxColourDialog colourDialog(this, &colourData);
					if (colourDialog.ShowModal() == wxID_OK)
					{
						// Set colour in controlpoint, preserve the alpha value
						colourData = colourDialog.GetColourData();
						wxColour c = colourData.GetColour();
						c.Set(c.Red(), c.Green(), c.Blue(), mMovingControlPoint->getColour().Alpha());
						mMovingControlPoint->setColour(c);
						Refresh();
					}
				}
			}
		break;

		case ID_ALPHA_CP_CTRL:
			{
				if (mMovingControlPoint)
				{
					int sliderWindowWidth = 400;
					wxPoint pos = GetParent()->GetParent()->GetPosition() + 
						wxPoint(0.5 * (GetParent()->GetParent()->GetSize().x - sliderWindowWidth), 2 * CONTROL_POINT_CONTROL_RECT_HEIGHT); // Not really generic is it?
					
					SliderDialog dlg(this, 
						wxID_ANY, 
						_("Alpha"), 
						pos, 
						wxSize(sliderWindowWidth, 80), 
						wxT("SliderDialog"), 
						mMovingControlPoint->getColour().Alpha(), 0, 255, 255);
					if (dlg.ShowModal() == wxID_OK)
					{
						wxColour col(mMovingControlPoint->getColour().Red(),
							mMovingControlPoint->getColour().Green(),
							mMovingControlPoint->getColour().Blue(),
							dlg.getValue() * 255);
						mMovingControlPoint->setColour(col);
						Refresh();
					}
				}
			}
		break;
	}
}
//-----------------------------------------------------------------------
void ControlPointColourControl::reset(void)
{
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
	for (it = mControlPointList.begin(); it != itEnd; ++it)
	{	
		delete *it;
	}
	mControlPointList.clear();

	if (mInitialControlPointList.empty())
	{
		// If no controlpoints are passed at all, add two controlpoints as default
		ControlPoint* cp = new ControlPoint(ControlPoint::CTRL_POINT_TYPE_SOLID, wxPoint(GetPosition().x, CONTROL_POINT_CONTROL_Y), false);
		mControlPointList.push_back(cp);
		cp = new ControlPoint(ControlPoint::CTRL_POINT_TYPE_SOLID, wxPoint(GetSize().x - CONTROL_POINT_RECT_SIZE - CONTROL_POINT_BORDER_SIZE_BOLD, CONTROL_POINT_CONTROL_Y), false);
		mControlPointList.push_back(cp);
	}
	else
	{
		addInitialControlpoints();
	}
		
	Refresh();
}
//-----------------------------------------------------------------------
void ControlPointColourControl::addInitialControlpoints(void)
{
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mInitialControlPointList.end();
	ControlPoint* cp = 0;
	for (it = mInitialControlPointList.begin(); it != itEnd; ++it)
	{
		int x = (*it)->getPosition().x;
		int y = (*it)->getPosition().y;
		if (it == mInitialControlPointList.begin())
		{
			cp = new ControlPoint(ControlPoint::CTRL_POINT_TYPE_SOLID, wxPoint(0, y), false); // Must start at left
			cp->setColour((*it)->getColour());
			mControlPointList.push_back(cp);
			if (x > 0)
			{
				cp = new ControlPoint(ControlPoint::CTRL_POINT_TYPE_SOLID, wxPoint(x, y), true, GetSize().x);
				cp->setColour((*it)->getColour());
				mControlPointList.push_back(cp);
			}
		}
		if (it == --mInitialControlPointList.end())
		{
			if (x < GetSize().x - CONTROL_POINT_RECT_SIZE - CONTROL_POINT_BORDER_SIZE_BOLD)
			{
				cp = new ControlPoint(ControlPoint::CTRL_POINT_TYPE_SOLID, wxPoint(x, y), true, GetSize().x);
				cp->setColour((*it)->getColour());
				mControlPointList.push_back(cp);
			}
			cp = new ControlPoint(ControlPoint::CTRL_POINT_TYPE_SOLID, 
					wxPoint(GetSize().x - CONTROL_POINT_RECT_SIZE - CONTROL_POINT_BORDER_SIZE_BOLD, y), false); // Must start at right
			cp->setColour((*it)->getColour());
			mControlPointList.push_back(cp);
		}
		if (it != mInitialControlPointList.begin() && it != --mInitialControlPointList.end())
		{
			cp = new ControlPoint(ControlPoint::CTRL_POINT_TYPE_SOLID, wxPoint(x, y), true, GetSize().x);
			cp->setColour((*it)->getColour());
			mControlPointList.push_back(cp);
		}
	}
}
//-----------------------------------------------------------------------
std::vector<ControlPoint*>& ControlPointColourControl::getControlPointList(void)
{
	return mControlPointList;
}
