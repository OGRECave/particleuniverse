/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseControlPoint.h"

//-----------------------------------------------------------------------
ControlPoint::ControlPoint(CTRL_POINT_TYPE type, const wxPoint& pos, const bool movable, float width, float min, float max) : 
	mCtrlPointType(type),
	mPos(pos),
	mMovable(movable),
	mColour(wxColor(*wxWHITE)),
	mMoving(false),
	mWidth(width),
	mXval(0.0f),
	mYval(0.0f),
	mXCenterVal(0.0f),
	mYCenterVal(0.0f)
{
	mBitmapArrow = new wxBitmap(ICONS_DIR + wxT("arrow_up.png"), wxBITMAP_TYPE_PNG);
	mBitmapClose = new wxBitmap(ICONS_DIR + wxT("close.png"), wxBITMAP_TYPE_PNG);
	mDiff = (max - min) / width;
}
//-----------------------------------------------------------------------
ControlPoint::~ControlPoint(void)
{
	delete mBitmapArrow;
	delete mBitmapClose;
}
//-----------------------------------------------------------------------
void ControlPoint::paint(wxPaintDC& dc)
{
	wxColour col;
	if (mCtrlPointType == CTRL_POINT_TYPE_SOLID)
	{
		col.Set(wxT("#000000"));
		dc.SetPen(wxPen(col, CONTROL_POINT_BORDER_SIZE, wxSOLID));
		dc.DrawBitmap(*mBitmapArrow, wxPoint(mPos.x, mPos.y - 0.5 * CONTROL_POINT_RECT_SIZE));
		dc.SetBrush(wxBrush(mColour, wxSOLID));
		dc.DrawRectangle(mPos.x, mPos.y, CONTROL_POINT_RECT_SIZE, CONTROL_POINT_RECT_SIZE);
	}
	else
	{
		// Assume it is CTRL_POINT_TYPE_GRID
		col.Set(wxT("#000000"));
		dc.SetPen(wxPen(col, 2, wxSOLID));
		dc.SetBrush(wxBrush(mColour, wxTRANSPARENT));
		int r =  + 0.5 * CONTROL_POINT_RECT_SIZE;
		dc.DrawCircle(mPos.x + r, mPos.y + r,  r);
	}
	if (mMovable)
	{
		col.Set(wxT("#000000"));
		dc.DrawBitmap(*mBitmapClose, wxPoint(mPos.x, mPos.y + CONTROL_POINT_RECT_SIZE));
		if (mMoving)
		{
			if (mCtrlPointType == CTRL_POINT_TYPE_SOLID)
			{
				float value = (mPos.x + mPos.x * (float)CONTROL_POINT_RECT_SIZE/mWidth) * mDiff; // Value must be within 0 to 1 and not 0 to 0.96
				//Ogre::String s = Ogre::StringConverter::toString(value, 2);
				Ogre::String s;
				trunc (value, s, 2);
				wxPoint textPos;
				textPos.y = mPos.y + CONTROL_POINT_RECT_SIZE;
				if (mPos.x > mWidth - 3 * CONTROL_POINT_RECT_SIZE)
				{
					textPos.x = mPos.x - CONTROL_POINT_RECT_SIZE;
				}
				else
				{
					textPos.x = mPos.x + CONTROL_POINT_RECT_SIZE;
				}
				dc.DrawRotatedText(ogre2wx(s), textPos, 90);
			}
			else
			{
				// Assume it is CTRL_POINT_TYPE_GRID
				float xValue = mXval;
				Ogre::String sX;
				trunc(xValue, sX, 2);

				float yValue = mYval;
				Ogre::String sY;
				trunc(yValue, sY, 2);

				Ogre::String s = sX + ", " + sY;
				wxPoint textPos;
				textPos.x = mPos.x - 2 * s.length();
				if (mXval > mXCenterVal)
				{
					textPos.x = mPos.x - 4 * s.length();
				}
				else
				{
					textPos.x = mPos.x + 0.5 * CONTROL_POINT_RECT_SIZE;
				}
				if (mYval > mYCenterVal)
				{
					textPos.y = mPos.y + 2 * CONTROL_POINT_RECT_SIZE;
				}
				else
				{
					textPos.y = mPos.y - CONTROL_POINT_RECT_SIZE;
				}
				dc.DrawRotatedText(ogre2wx(s), textPos, 0);
			}
		}
	}
}
//-----------------------------------------------------------------------
const wxPoint& ControlPoint::getPosition(void) const
{
	return mPos;
}
//-----------------------------------------------------------------------
void ControlPoint::setPosition(const wxPoint& pos)
{
	mPos = pos;
}
//-----------------------------------------------------------------------
const wxColour& ControlPoint::getColour(void) const
{
	return mColour;
}
//-----------------------------------------------------------------------
void ControlPoint::setColour(const wxColour& col)
{
	mColour = col;
}
//-----------------------------------------------------------------------
bool ControlPoint::isMovable(void) const
{
	return mMovable;
}
//-----------------------------------------------------------------------
bool ControlPoint::isMoving(void) const
{
	return mMoving;
}
//-----------------------------------------------------------------------
void ControlPoint::setMoving(bool moving)
{
	mMoving = moving;
}
//-----------------------------------------------------------------------
float ControlPoint::getXval(void) const
{
	return mXval;
}
//-----------------------------------------------------------------------
void ControlPoint::setXval(float xval)
{
	mXval = xval;
}
//-----------------------------------------------------------------------
float ControlPoint::getYval(void) const
{
	return mYval;
}
//-----------------------------------------------------------------------
void ControlPoint::setYval(float yval)
{
	mYval = yval;
}
//-----------------------------------------------------------------------
void ControlPoint::setXCenterVal(float xCenterVal)
{
	mXCenterVal = xCenterVal;
}
//-----------------------------------------------------------------------
void ControlPoint::setYCenterVal(float yCenterVal)
{
	mYCenterVal = yCenterVal;
}
//-----------------------------------------------------------------------
void trunc (float value, std::string& s, int maxDecimals)
{
	if (value > -0.0001 && value < 0.0001)
	{
		value = 0.00f;
	}

	std::stringstream ss;
	if (value > -10.0f && value < 10.0f)
	{
		ss << setiosflags(std::ios::fixed) << std::setprecision(maxDecimals) << value;
	}
	else
	{
		--maxDecimals;
		ss << setiosflags(std::ios::fixed) << std::setprecision(maxDecimals) << value;
	}
	s = ss.str();
}
