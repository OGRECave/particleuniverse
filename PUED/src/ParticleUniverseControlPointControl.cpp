/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseControlPointControl.h"
#include <wx/colordlg.h>

//-----------------------------------------------------------------------
ControlPointControl::ControlPointControl(std::vector<ControlPoint*>& v,
			wxWindow* parent, 
			wxWindowID id, 
			const wxPoint& pos, 
			const wxSize& size, 
			const wxString& name) : 
	wxControl (parent, id, pos, size, 0, wxDefaultValidator, name),
	mParent(parent),
	mMovingControlPoint(0),
	mMoving(false),
	mInitialControlPointList(v),
	mMinXGraph(0.0f),
	mMaxXGraph(1.0f),
	mMinYGraph(0.0f),
	mMaxYGraph(1.0f),
	mNumberOfGridLines(20),
	mLinearInterpolation(true)
{
	// Fill the controlpoints
	reset();

	// Connect the events
	Connect(wxEVT_PAINT, wxPaintEventHandler(ControlPointControl::OnPaint));
	Connect(wxEVT_ERASE_BACKGROUND , wxEraseEventHandler(ControlPointControl::OnClear));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ControlPointControl::OnMouseLButtonPressed));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ControlPointControl::OnMouseLButtonReleased));
	Connect(wxEVT_MOTION, wxMouseEventHandler(ControlPointControl::OnMove));
	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(ControlPointControl::OnMouseRButtonPressed));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(ControlPointControl::OnWindowEnter));
}
//-----------------------------------------------------------------------
ControlPointControl::~ControlPointControl(void)
{
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
	for (it = mControlPointList.begin(); it != itEnd; ++it)
	{
		delete *it;
	}
	mControlPointList.clear();
}
//-----------------------------------------------------------------------
void ControlPointControl::OnPaint(wxPaintEvent& event)
{
	SetPosition(wxPoint(0, 0)); // Always set the canvas to the original position
	wxPaintDC dc(this);

	// Draw the graph
	drawGraph(dc);

	// Sort the controlpoints
	sortControlPoints();

	// Draw lines between controlpoints
	if (mControlPointList.size() > 1)
	{
		wxColour col;
		col.Set(wxT("#000000"));
		dc.SetPen(wxPen(col, 2, wxSOLID));
		dc.SetBrush(wxBrush(col, wxSOLID));
		int half = 0.5 * CONTROL_POINT_RECT_SIZE;
		if (mLinearInterpolation)
		{
			std::vector<ControlPoint*>::iterator itFirst = mControlPointList.begin();
			std::vector<ControlPoint*>::iterator itNext;
			itNext = itFirst;
			++itNext;
			int xFirst = 0;
			int yFirst = 0;
			int xNext = 0;
			int yNext = 0;
			std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
			while (itNext != itEnd)
			{
				int xFirst = (*itFirst)->getPosition().x + half;
				int yFirst = (*itFirst)->getPosition().y + half;;
				int xNext = (*itNext)->getPosition().x + half;;
				int yNext = (*itNext)->getPosition().y + half;;
				dc.DrawLine(xFirst, yFirst, xNext, yNext);
				itFirst = itNext;
				++itNext;	
			}
		}
		else
		{
			Ogre::SimpleSpline spline;
			std::vector<ControlPoint*>::iterator it;
			std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
			for (it = mControlPointList.begin(); it != itEnd; ++it)
			{
				spline.addPoint(Ogre::Vector3((*it)->getPosition().x, (*it)->getPosition().y, 0));
			}
			int minX = 36;
			int maxX = GetSize().x - 22;
			Ogre::Vector3 interpolatedStart;
			Ogre::Vector3 interpolatedEnd;
			float count = 0.0f;
			float delta = 1.0 / (maxX - minX);
			interpolatedStart = spline.interpolate(count);
			count += delta;
			for (int x = minX; x < maxX; ++x)
			{
				interpolatedEnd = spline.interpolate(count);
				dc.DrawLine(interpolatedStart.x + half, 
					interpolatedStart.y + half, 
					interpolatedEnd.x + half, 
					interpolatedEnd.y + half);
				count += delta;
				interpolatedStart = interpolatedEnd;
			}
		}
	}

	// Draw all control points
	float xCenterVal = 0.5 * (mMaxXGraph + mMinXGraph);
	float yCenterVal = 0.5 * (mMaxYGraph + mMinYGraph);
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
	for (it = mControlPointList.begin(); it != itEnd; ++it)
	{
		(*it)->setXCenterVal(xCenterVal);
		(*it)->setYCenterVal(yCenterVal);
		(*it)->paint(dc);
	}
}
//-----------------------------------------------------------------------
void ControlPointControl::OnClear(wxEraseEvent& event)
{
	wxDC* dc = event.GetDC();
	wxColour col = GetBackgroundColour();
	dc->SetPen(wxPen(col, CONTROL_POINT_BORDER_SIZE, wxSOLID));
	dc->SetBrush(wxBrush(col, wxSOLID));
	dc->DrawRectangle(wxPoint(0, 0), GetSize());
}
//------------------------------------------------------------------------------
void ControlPointControl::OnMouseLButtonPressed(wxMouseEvent& event)
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
	if (event.GetPosition().y < 20 - 0.5 * CONTROL_POINT_RECT_SIZE || 
		event.GetPosition().y > GetSize().y - 19 - 0.5 * CONTROL_POINT_RECT_SIZE || 
		event.GetPosition().x < 36 - 0.5 * CONTROL_POINT_RECT_SIZE || 
		event.GetPosition().x > GetSize().x - 22 - 0.5 * CONTROL_POINT_RECT_SIZE)
		return;

	// Create a new control point
	ControlPoint* cp = new ControlPoint(
		ControlPoint::CTRL_POINT_TYPE_GRID,
		event.GetPosition(),
		true);

	float xDiff = (float)(event.GetPosition().x - 36) / (float)(GetSize().x - 58);
	//float xValue = mMinXGraph + xDiff * (abs(mMinXGraph) + abs(mMaxXGraph));
	float xValue = mMinXGraph + xDiff * (mMaxXGraph - mMinXGraph);
	cp->setXval(xValue);
	float yDiff = (float)(event.GetPosition().y + 0.5 * CONTROL_POINT_RECT_SIZE - 20) / (float)(GetSize().y - 39);
	//float yValue = mMaxYGraph - yDiff * (abs(mMinYGraph) + abs(mMaxYGraph));
	float yValue = mMaxYGraph - yDiff * (mMaxYGraph - mMinYGraph);
	cp->setYval(yValue);
	mControlPointList.push_back(cp);
	mMovingControlPoint = cp;
	mMoving = true;
	Refresh();
}
//------------------------------------------------------------------------------
void ControlPointControl::OnMouseLButtonReleased(wxMouseEvent& event)
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
void ControlPointControl::OnMove(wxMouseEvent& event)
{
	if (!mMoving)
		return;

	// Moving is only allowed in the 'zone'
	if (event.GetPosition().y < 20 - 0.5 * CONTROL_POINT_RECT_SIZE || 
		event.GetPosition().y > GetSize().y - 19 - 0.5 * CONTROL_POINT_RECT_SIZE || 
		event.GetPosition().x < 36 - 0.5 * CONTROL_POINT_RECT_SIZE || 
		event.GetPosition().x > GetSize().x - 22 - 0.5 * CONTROL_POINT_RECT_SIZE)
		return;

	if (mMovingControlPoint && mMovingControlPoint->isMovable())
	{
		mMoving = true;
		mMovingControlPoint->setMoving(true);
		Freeze();

		float xDiff = (float)(event.GetPosition().x + 0.5 * CONTROL_POINT_RECT_SIZE - 36) / (float)(GetSize().x - 58);
		//float xValue = mMinXGraph + xDiff * (abs(mMinXGraph) + abs(mMaxXGraph));
		float xValue = mMinXGraph + xDiff * (mMaxXGraph - mMinXGraph);
		mMovingControlPoint->setXval(xValue);
		float yDiff = (float)(event.GetPosition().y + 0.5 * CONTROL_POINT_RECT_SIZE - 20) / (float)(GetSize().y - 39);
		//float yValue = mMaxYGraph - yDiff * (abs(mMinYGraph) + abs(mMaxYGraph));
		float yValue = mMaxYGraph - yDiff * (mMaxYGraph - mMinYGraph);
		mMovingControlPoint->setYval(yValue);
		mMovingControlPoint->setPosition(wxPoint(event.GetPosition()));
		Refresh();
		Thaw();
	}
}
//------------------------------------------------------------------------------
void ControlPointControl::OnMouseRButtonPressed(wxMouseEvent& event)
{
}
//------------------------------------------------------------------------------
void ControlPointControl::OnWindowEnter(wxMouseEvent& event)
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
//-----------------------------------------------------------------------
void ControlPointControl::reset(void)
{
	// Set original control points
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
	for (it = mControlPointList.begin(); it != itEnd; ++it)
	{	
		delete *it;
	}
	mControlPointList.clear();
	addInitialControlpoints();

	// TODO: Set original interpolation type
		
	Refresh();
}
//-----------------------------------------------------------------------
void ControlPointControl::addInitialControlpoints(void)
{
	// Determine min/max values
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mInitialControlPointList.end();
	for (it = mInitialControlPointList.begin(); it != itEnd; ++it)
	{
		mMaxXGraph = std::max(mMaxXGraph, (*it)->getXval());
		mMaxYGraph = std::max(mMaxYGraph, (*it)->getYval());
		mMinXGraph = std::min(mMinXGraph, (*it)->getXval());
		mMinYGraph = std::min(mMinYGraph, (*it)->getYval());
	}
	//mMaxXGraph = std::floor(mMaxXGraph);
	mMaxXGraph = std::ceil(mMaxXGraph);
	//mMaxYGraph = std::floor(mMaxYGraph);
	mMaxYGraph = std::ceil(mMaxYGraph);
	mMinXGraph = std::floor(mMinXGraph);
	mMinYGraph = std::floor(mMinYGraph);

	// Copy controlpoints
	ControlPoint* cp = 0;
	for (it = mInitialControlPointList.begin(); it != itEnd; ++it)
	{
		cp = new ControlPoint(ControlPoint::CTRL_POINT_TYPE_GRID, (*it)->getPosition(), true);

		// Set values
		cp->setXval((*it)->getXval());
		cp->setYval((*it)->getYval());

		// Calculate positions
		float xDiff = ((float)(GetSize().x - 58) / (mMaxXGraph - mMinXGraph));
		float x = 36 + xDiff * ((*it)->getXval() - mMinXGraph) - 0.5 * CONTROL_POINT_RECT_SIZE;
		float yDiff = ((float)(GetSize().y - 39) / (mMaxYGraph - mMinYGraph));
		float y = 20 + yDiff * (mMaxYGraph - (*it)->getYval()) - 0.5 * CONTROL_POINT_RECT_SIZE;
		cp->setPosition(wxPoint(x, y));
		mControlPointList.push_back(cp);
	}
}
//-----------------------------------------------------------------------
std::vector<ControlPoint*>& ControlPointControl::getControlPointList(void)
{
	return mControlPointList;
}
//-----------------------------------------------------------------------
void ControlPointControl::drawGraph(wxPaintDC& dc)
{
	// Draw grid
	wxColour col(*wxBLACK);
	dc.SetPen(wxPen(col, 1, wxSOLID));
	dc.SetBrush(wxBrush(col, wxSOLID));
	float xValue = mMinXGraph;
	//float deltaXValue = (abs(mMinXGraph) + abs(mMaxXGraph)) / mNumberOfGridLines;
	float deltaXValue = (mMaxXGraph - mMinXGraph) / mNumberOfGridLines;
	int delta = (GetSize().x - 58) / mNumberOfGridLines;
	int count = 0;

	// Draw vertical lines
	int maxX = GetSize().x - 22;
	for (int x = 36; x < maxX; x+=delta)
	{
		dc.DrawLine(wxPoint(x, 20), wxPoint(x, GetSize().y - 20));
		if (!(count % 2))
		{
			// Only print at the even line numbers, but not the first at the right
			Ogre::String s;
			trunc(xValue, s, 1);
			dc.DrawRotatedText(ogre2wx(s), wxPoint(x - 8, 0), 0);
		}
		xValue += deltaXValue;
		count++;
	}

	float yValue = mMaxYGraph;
	//float deltaYValue = (abs(mMinYGraph) + abs(mMaxYGraph)) / mNumberOfGridLines;
	float deltaYValue = (mMaxYGraph - mMinYGraph) / mNumberOfGridLines;
	delta = (GetSize().y - 36) / mNumberOfGridLines;
	count = 0;

	// Draw horizontal lines
	int maxY = GetSize().y;
	for (int y = 20; y < maxY; y+=delta)
	{
		dc.DrawLine(wxPoint(36, y), wxPoint(GetSize().x - 24, y));
		if (!(count % 2))
		{
			// Only print at the even line numbers, but not the first at the top
			Ogre::String s;
			trunc(yValue, s, 1);
			dc.DrawRotatedText(ogre2wx(s), wxPoint(4, y - 8), 0);
		}
		yValue -= deltaYValue;
		count++;
	}

	// Draw surrounding rectangle
	dc.SetPen(wxPen(col, 2, wxSOLID));
	dc.SetBrush(wxBrush(col, wxTRANSPARENT));
	dc.DrawRectangle(36, 20, GetSize().x - 58, GetSize().y - 39);
}
//-----------------------------------------------------------------------
float ControlPointControl::getMinXGraph(void) const
{
	return mMinXGraph;
}
//-----------------------------------------------------------------------
float ControlPointControl::getMinYGraph(void) const
{
	return mMinYGraph;
}
//-----------------------------------------------------------------------
float ControlPointControl::getMaxXGraph(void) const
{
	return mMaxXGraph;
}
//-----------------------------------------------------------------------
float ControlPointControl::getMaxYGraph(void) const
{
	return mMaxYGraph;
}
//-----------------------------------------------------------------------
void ControlPointControl::scaleMinMaxXGraph(float scale)
{
	mMinXGraph *= scale;
	mMaxXGraph *= scale;
	float diff = ((float)(GetSize().x - 58) / (mMaxXGraph - mMinXGraph));
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
	for (it = mControlPointList.begin(); it != itEnd; ++it)
	{
		int rescaleX = 36 + diff * ((*it)->getXval() - mMinXGraph) - 0.5 * CONTROL_POINT_RECT_SIZE;
		(*it)->setPosition(wxPoint(rescaleX, (*it)->getPosition().y));
	}
}
//-----------------------------------------------------------------------
void ControlPointControl::scaleMinMaxYGraph(float scale)
{
	mMinYGraph *= scale;
	mMaxYGraph *= scale;
	float diff = ((float)(GetSize().y - 39) / (mMaxYGraph - mMinYGraph));
	std::vector<ControlPoint*>::iterator it;
	std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
	for (it = mControlPointList.begin(); it != itEnd; ++it)
	{
		int rescaleY = 20 + diff * (mMaxYGraph - (*it)->getYval()) - 0.5 * CONTROL_POINT_RECT_SIZE;
		(*it)->setPosition(wxPoint((*it)->getPosition().x, rescaleY));
	}
}
//------------------------------------------------------------------------------
void ControlPointControl::scrollMinMaxXGraph(float scroll)
{
	//float deltaXValue = (abs(mMinXGraph) + abs(mMaxXGraph)) / mNumberOfGridLines;
	float deltaXValue = (mMaxXGraph - mMinXGraph) / mNumberOfGridLines;
	deltaXValue*=2;
	int delta = (GetSize().x - 58) / mNumberOfGridLines;
	delta*=2;
	if (scroll < 0.0f)
	{
		mMinXGraph+=deltaXValue;
		mMaxXGraph+=deltaXValue;
		std::vector<ControlPoint*>::iterator it;
		std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
		for (it = mControlPointList.begin(); it != itEnd; ++it)
		{
			int rescaleX = (*it)->getPosition().x - delta;
			(*it)->setPosition(wxPoint(rescaleX, (*it)->getPosition().y));
		}
	}
	else
	{
		mMinXGraph-=deltaXValue;
		mMaxXGraph-=deltaXValue;
		std::vector<ControlPoint*>::iterator it;
		std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
		for (it = mControlPointList.begin(); it != itEnd; ++it)
		{
			int rescaleX = (*it)->getPosition().x + delta;
			(*it)->setPosition(wxPoint(rescaleX, (*it)->getPosition().y));
		}
	}
}
//------------------------------------------------------------------------------
void ControlPointControl::scrollMinMaxYGraph(float scroll)
{
	//float deltaYValue = (abs(mMinYGraph) + abs(mMaxYGraph)) / mNumberOfGridLines;
	float deltaYValue = (mMaxYGraph - mMinYGraph) / mNumberOfGridLines;
	deltaYValue*=2;
	int delta = (GetSize().y - 36) / mNumberOfGridLines;
	delta*=2;
	if (scroll < 0.0f)
	{
		mMinYGraph+=deltaYValue;
		mMaxYGraph+=deltaYValue;
		std::vector<ControlPoint*>::iterator it;
		std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
		for (it = mControlPointList.begin(); it != itEnd; ++it)
		{
			int rescaleY = (*it)->getPosition().y + delta;
			(*it)->setPosition(wxPoint((*it)->getPosition().x, rescaleY));
		}
	}
	else
	{
		mMinYGraph-=deltaYValue;
		mMaxYGraph-=deltaYValue;
		std::vector<ControlPoint*>::iterator it;
		std::vector<ControlPoint*>::iterator itEnd = mControlPointList.end();
		for (it = mControlPointList.begin(); it != itEnd; ++it)
		{
			int rescaleY = (*it)->getPosition().y - delta;
			(*it)->setPosition(wxPoint((*it)->getPosition().x, rescaleY));
		}
	}
}
//------------------------------------------------------------------------------
void ControlPointControl::sortControlPoints(void)
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
//------------------------------------------------------------------------------
bool ControlPointControl::useLinearInterpolation(void)
{
	return mLinearInterpolation;
}
//------------------------------------------------------------------------------
void ControlPointControl::setLinearInterpolation(bool linearInterpolation)
{
	mLinearInterpolation = linearInterpolation;
	Refresh();
}
