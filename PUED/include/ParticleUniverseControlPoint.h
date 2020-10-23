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

#ifndef __PUED_CONTROLPOINT_
#define __PUED_CONTROLPOINT_


/**	function to format a string (used for control points and control point grid only
*/
void trunc (float value, std::string& s, int maxDecimals);

/**	Component that represents a controlpoint, needed in the ControlPointColourControl
*/
static const int CONTROL_POINT_RECT_SIZE = 16;
static const int CONTROL_POINT_BORDER_SIZE = 1;
static const int CONTROL_POINT_BORDER_SIZE_BOLD = 4;
class ControlPoint
{
	public:
		enum CTRL_POINT_TYPE
		{
			CTRL_POINT_TYPE_SOLID,
			CTRL_POINT_TYPE_GRID,
		};
		ControlPoint(CTRL_POINT_TYPE type, const wxPoint& pos, const bool movable = true, float width = 1.0f, float min = 0.0f, float max = 1.0f);
		virtual ~ControlPoint(void);
		void paint(wxPaintDC& dc);
		const wxPoint& getPosition(void) const;
		void setPosition(const wxPoint& pos);
		const wxColour& getColour(void) const;
		void setColour(const wxColour& col);
		bool isMovable(void) const;
		bool isMoving(void) const;
		void setMoving(bool moving);
		float getXval(void) const;
		void setXval(float xval);
		float getYval(void) const;
		void setYval(float yval);
		void setXCenterVal(float xCenterVal);
		void setYCenterVal(float yCenterVal);


	protected:
		CTRL_POINT_TYPE mCtrlPointType;
		wxPoint mPos;
		bool mMovable;
		wxBitmap* mBitmapArrow;
		wxBitmap* mBitmapClose;
		wxColour mColour;
		bool mMoving;
		float mDiff;
		float mWidth;
		float mXval;
		float mYval;
		float mXCenterVal;
		float mYCenterVal;
};

#endif