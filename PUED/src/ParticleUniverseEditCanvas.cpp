/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "wx/ogre/utils.h"
#include "ParticleUniverseEditCanvas.h"
#include "ParticleUniverseEditTab.h"

//-----------------------------------------------------------------------
EditCanvas::EditCanvas(wxMDIParentFrame* parent) : 
		wxMDIChildFrame(
		parent, 
		wxID_ANY, 
		wxT(""), 
		wxPoint(0, 0), 
		parent->GetSize(), 
		0, 
		wxT("canvas")),
		mMouseConnector(0),
		mMousePosition(wxPoint(0, 0)),
		mMousePositionAdjustment(wxPoint(0, 0)),
		mSelectionMode(SM_SELECT_NONE),
		mStartPositionSelection(wxPoint(0, 0)),
		mEndPositionSelection(wxPoint(0, 0))
{
	Connect(wxEVT_PAINT, wxPaintEventHandler(EditCanvas::OnPaint));
	Connect(wxEVT_ACTIVATE, wxActivateEventHandler(EditCanvas::OnActivate));
	Connect(wxEVT_MOTION, wxMouseEventHandler(EditCanvas::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(EditCanvas::OnMouseLButtonPressed));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(EditCanvas::OnMouseLButtonReleased));
	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(EditCanvas::OnMouseRButtonPressed));
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(EditCanvas::OnKeyPressed));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(EditCanvas::OnMouseWheel));
}
//-----------------------------------------------------------------------
void EditCanvas::OnPaint(wxPaintEvent& event)
{
	SetPosition(wxPoint(0, 0)); // Always set the canvas to the original position
	wxPaintDC dc(this);
	wxColour col;
	col.Set(wxT("#000000"));

	// Draw all connections between components
	dc.SetPen(wxPen(col, 1, wxSOLID));
	std::vector<DrawConnector*>::iterator it;
	std::vector<DrawConnector*>::iterator itEnd = mDrawConnectors.end();
	for (it = mDrawConnectors.begin(); it != itEnd; ++it)
	{
		(*it)->drawLine(this, &dc);
	}

	// Draw the mouse connector (if available)
	if (mMouseConnector)
	{
		mMouseConnector->drawLine(this, &dc);
	}

	// Draw the selection box (if available)
	if (mSelectionMode == SM_SELECTING)
	{
		// Draw the selection box while moving
		dc.SetPen(wxPen(col, 1, wxSHORT_DASH));
		wxPoint points[5];
		points[0] = wxPoint(mStartPositionSelection.x, mStartPositionSelection.y);
		points[1] = wxPoint(mMousePosition.x, mStartPositionSelection.y);
		points[2] = wxPoint(mMousePosition.x, mMousePosition.y);
		points[3] = wxPoint(mStartPositionSelection.x, mMousePosition.y);
		points[4] = wxPoint(mStartPositionSelection.x, mStartPositionSelection.y);
		dc.DrawLines(5, points);
	} else if (mSelectionMode == SM_SELECTED)
	{
		// Draw the selected box
		dc.SetPen(wxPen(col, 1, wxSHORT_DASH));
		wxPoint points[5];
		points[0] = wxPoint(mStartPositionSelection.x, mStartPositionSelection.y);
		points[1] = wxPoint(mEndPositionSelection.x, mStartPositionSelection.y);
		points[2] = wxPoint(mEndPositionSelection.x, mEndPositionSelection.y);
		points[3] = wxPoint(mStartPositionSelection.x, mEndPositionSelection.y);
		points[4] = wxPoint(mStartPositionSelection.x, mStartPositionSelection.y);
		dc.DrawLines(5, points);
	}
}
//-----------------------------------------------------------------------
void EditCanvas::OnActivate(wxActivateEvent& event)
{
	// Put it lower in the hierarchy so all other windows are not covered by the canvas
	Lower();
	event.Skip();
}
//-----------------------------------------------------------------------
void EditCanvas::connect(EditComponent* node1, 
	EditComponent* node2, 
	ComponentRelation relation, 
	const Ogre::String colourCode,
	int lineStyle)
{
	DrawConnector* drawConnector = new DrawConnector(node1, node2, relation, colourCode, lineStyle);
	mDrawConnectors.push_back(drawConnector);
}
//-----------------------------------------------------------------------
std::vector<DrawConnector*>& EditCanvas::getDrawConnections(void)
{
	return mDrawConnectors;
}
//-----------------------------------------------------------------------
unsigned int EditCanvas::getNumberOfConnections(void) const
{
	return mDrawConnectors.size();
}
//-----------------------------------------------------------------------
void EditCanvas::removeConnection(EditComponent* node)
{
	if (mDrawConnectors.empty())
		return;

	std::vector<DrawConnector*>::iterator it;
	std::vector<DrawConnector*> temp;
	DrawConnector* drawConnector = 0;
	for(it = mDrawConnectors.begin(); it != mDrawConnectors.end(); ++it)
	{
		drawConnector = *it;
		if (drawConnector->hasNode(node))
		{
			delete drawConnector;
		}
		else
		{
			temp.push_back(drawConnector);
		}
	}
	mDrawConnectors = temp;
}
//-----------------------------------------------------------------------
void EditCanvas::removeConnection(EditComponent* node1, EditComponent* node2, ComponentRelation relation)
{
	if (mDrawConnectors.empty())
		return;

	std::vector<DrawConnector*>::iterator it;
	DrawConnector* drawConnector = 0;
	for(it = mDrawConnectors.begin(); it != mDrawConnectors.end(); ++it)
	{
		drawConnector = *it;
		if (drawConnector->hasNodesAndRelation(node1, node2, relation))
		{
			delete drawConnector;
			mDrawConnectors.erase(it);
			return;
		}
	}
}
//-----------------------------------------------------------------------
void EditCanvas::createMouseConnector(wxWindow* node)
{
	mMouseConnector = new DrawConnector(node, mMousePosition);
}
//-----------------------------------------------------------------------
void EditCanvas::destroyMouseConnector(void)
{
	if (mMouseConnector)
	{
		delete mMouseConnector;
	}
	mMouseConnector = 0;
}
//------------------------------------------------------------------------------
void EditCanvas::OnMouseMove(wxMouseEvent& event)
{
	wxPoint previousMousePosition = mMousePosition;
	mMousePosition = event.GetPosition();
	if (mMouseConnector)
	{
		mMouseConnector->setPosition(mMousePosition);
		Refresh();
	}
	else if (mSelectionMode == SM_MOVING)
	{
		moveSelectedComponents(previousMousePosition, mMousePosition);
		Refresh();
	} 
	else if (mSelectionMode == SM_SELECTING)
	{
		Refresh();
	}
}
//------------------------------------------------------------------------------
void EditCanvas::OnMouseLButtonPressed(wxMouseEvent& event)
{
	// Start possible action for a selectionbox
	if (mSelectionMode == SM_SELECT_NONE)
	{
		mSelectionMode = SM_SELECTING;
		mStartPositionSelection = mMousePosition;
	}
	else if (mSelectionMode == SM_SELECTED)
	{
		mSelectionMode = SM_MOVING;
	}
}
//------------------------------------------------------------------------------
void EditCanvas::OnMouseLButtonReleased(wxMouseEvent& event)
{
	if (mSelectionMode == SM_SELECTING && mStartPositionSelection != mMousePosition)
	{
		// Rectangle has been selected, because the mouse has been been (irt the start position)
		mSelectionMode = SM_SELECTED;
		mEndPositionSelection = mMousePosition;
		selectComponents();
		wxCursor connectCursor = wxCursor(wxImage(ICONS_DIR + wxT("move.png")));
		SetCursor(connectCursor);
		SetFocus();
	}
	else
	{
		mSelectionMode = SM_SELECT_NONE;
		wxCursor connectCursor = wxNullCursor;
		SetCursor(connectCursor);
	}
}
//------------------------------------------------------------------------------
void EditCanvas::OnMouseRButtonPressed(wxMouseEvent& event)
{
	// Reset selection mode (state is kept by canvas)
	mSelectionMode = SM_SELECT_NONE;
	mStartPositionSelection = wxPoint(0, 0);
	mEndPositionSelection = wxPoint(0, 0);
	mSelectedComponents.clear();

	// Reset connection mode (state is kept by parent)
	(static_cast<EditTab*>(GetParent()))->resetConnectionMode();
	
	Refresh(); // Removes the selection box
}
//------------------------------------------------------------------------------
void EditCanvas::OnKeyPressed(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_DELETE && mSelectionMode == SM_SELECTED)
	{
		wxMessageDialog m(this, _("Are you sure you want to delete this group?"), _("Warning"), wxOK | wxCANCEL);
		if (m.ShowModal() == wxID_OK)
		{
			// Delete selected components
			deleteSelectedComponents();
		}

		mSelectionMode = SM_SELECT_NONE;
		mStartPositionSelection = wxPoint(0, 0);
		mEndPositionSelection = wxPoint(0, 0);
		mSelectedComponents.clear();
		wxCursor connectCursor = wxNullCursor;
		SetCursor(connectCursor);
		Refresh();
	}
	else
	{
		(static_cast<EditTab*>(GetParent()))->OnKeyPressed(event);
	}
}
//------------------------------------------------------------------------------
void EditCanvas::OnMouseWheel(wxMouseEvent& event)
{
	// Delegate to the parent
	(static_cast<EditTab*>(GetParent()))->OnMouseWheel(event);
}
//------------------------------------------------------------------------------
void EditCanvas::selectComponents(void)
{
	mSelectedComponents.clear();
	int x1, y1;
	int x2, y2;
	std::vector<EditComponent*> allComponents = (static_cast<EditTab*>(GetParent()))->getComponents();
	std::vector<EditComponent*>::iterator it;
	std::vector<EditComponent*>::iterator itEnd = allComponents.end();
	for (it = allComponents.begin(); it != itEnd; ++it)
	{
		EditComponent* component = *it;
		x1 = component->GetPosition().x;
		y1 = component->GetPosition().y;
		x2 = x1 + component->GetSize().GetWidth();
		y2 = y1 + component->GetSize().GetHeight();

		if (x1 > mStartPositionSelection.x &&
			x2 < mEndPositionSelection.x &&
			y1 > mStartPositionSelection.y &&
			y2 < mEndPositionSelection.y)
		{
			// Selection box starts in upper left corner and ends in lower right corner
			mSelectedComponents.push_back(component);
		}
		else if (x1 > mEndPositionSelection.x &&
			x2 < mStartPositionSelection.x &&
			y1 > mEndPositionSelection.y &&
			y2 < mStartPositionSelection.y)
		{
			// Selection box starts in lower right corner and ends in upper left corner
			mSelectedComponents.push_back(component);
		}
	}
}
//------------------------------------------------------------------------------
void EditCanvas::moveSelectedComponents(const wxPoint& previousMousePosition, const wxPoint& mMousePosition)
{
	if (mSelectedComponents.empty())
		return;

	wxPoint diff = mMousePosition - previousMousePosition;
	std::vector<EditComponent*>::iterator it;
	std::vector<EditComponent*>::iterator itEnd = mSelectedComponents.end();
	for (it = mSelectedComponents.begin(); it != itEnd; ++it)
	{
		(*it)->SetPosition((*it)->GetPosition() + diff);
	}
}
//------------------------------------------------------------------------------
void EditCanvas::deleteSelectedComponents(void)
{
	if (mSelectedComponents.empty())
		return;

	std::vector<EditComponent*>::iterator it;
	std::vector<EditComponent*>::iterator itEnd = mSelectedComponents.end();
	for (it = mSelectedComponents.begin(); it != itEnd; ++it)
	{
		(*it)->Close(true);
	}
	mSelectedComponents.clear();
}
//------------------------------------------------------------------------------
void EditCanvas::adjustMousePosition(const wxPoint& componentPosition,  const wxPoint& mousePosition)
{
	// Only adjust if a specific action is going on
	if (mSelectionMode == SM_SELECT_NONE && !mMouseConnector)
		return;

	// Adjust the mouse and correct it with the canvas position (the componentPosition has not taken this into account)
	mMousePosition = componentPosition + mousePosition - GetPosition();
	if (mMouseConnector)
	{
		mMouseConnector->setPosition(mMousePosition);
	}
	Refresh(); // To update the selection box or the conection line
}

