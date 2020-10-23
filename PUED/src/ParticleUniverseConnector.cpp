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
#include "ParticleUniverseConnector.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
DrawConnector::DrawConnector(wxWindow* node1,
							 wxWindow* node2,
							 ComponentRelation relation,
							 const Ogre::String& colourCode,
							 int lineStyle) : 
	mNode1(node1), 
	mNode2(node2), 
	mRelation(relation),
	mColourCode(colourCode),
	mLineStyle(lineStyle),
	usePosition(false), 
	mPosition(wxPoint(0, 0)),
	xOffsetMin(24)
{
}
//-----------------------------------------------------------------------
DrawConnector::DrawConnector(wxWindow* node, 
							 wxPoint& position,
							 const Ogre::String& colourCode,
							 int lineStyle) : 
	mNode1(node), 
	mNode2(0), 
	mColourCode(colourCode),
	mLineStyle(lineStyle),
	usePosition(true), 
	mPosition(position),
	xOffsetMin(24)
{
}
//-----------------------------------------------------------------------
void DrawConnector::drawLine(wxWindow* canvas, wxPaintDC* dc)
{
	/** The canvas is also an MDI Child and if the parent frame scrolls, the canvas' position
		moves at the same rate as the nodes. Because the 'startposition' of the canvas is (0, 0),
		the canvas position that is used here is an offset/relative position in relation to the scrollbars.
	*/
	wxColor col;
	col.Set(ogre2wx(mColourCode));
	dc->SetPen(wxPen(col, 2, mLineStyle));

	if (usePosition)
	{
		wxPoint points[3];
		int x1 = mNode1->GetPosition().x - canvas->GetPosition().x;
		int y1 = mNode1->GetPosition().y - canvas->GetPosition().y + 0.5 * mNode1->GetSize().GetHeight();
		int x2 = mPosition.x;
		int y2 = mPosition.y;
		int xOffset = 0.25 * Ogre::Math::Abs(x1 - x2);
		int x3 = x1 - xOffset;
		int y3 = y1;

		if (x2 > x1)
		{
			x1 += mNode1->GetSize().GetWidth();
			x3 = x1 + xOffset;
		}
		points[0] = wxPoint(x1, y1);
		points[1] = wxPoint(x3, y3);
		points[2] = wxPoint(x2, y2);
		dc->DrawSpline(3, points);
	}
	else
	{
		wxPoint points[4];
		int x1 = mNode1->GetPosition().x - canvas->GetPosition().x;
		int y1 = mNode1->GetPosition().y - canvas->GetPosition().y; 
		int x2 = mNode2->GetPosition().x - canvas->GetPosition().x; 
		int y2 = mNode2->GetPosition().y - canvas->GetPosition().y;
		int width1 = mNode1->GetSize().GetWidth();
		int height1 = mNode1->GetSize().GetHeight();
		int width2 = mNode2->GetSize().GetWidth();
		int height2 = mNode2->GetSize().GetHeight();
		int x3, y3;
		int x4, y4;

		y1 = y1 + 0.5 * height1;
		y2 = y2 + 0.5 * height2;
		y3 = y1;
		y4 = y2;

		// Determine widest rectangle
		if (width2 > width1)
		{
			// Swap
			x1 += x2; // = x1 + x2
			x2 = x1 - x2; // = x1 + x2 - x2 = x1
			x1 -= x2; // = x1 + x2 - x1 = x2

			y1 += y2;
			y2 = y1 - y2;
			y1 -= y2;

			y3 += y4;
			y4 = y3 - y4;
			y3 -= y4;

			width1 += width2;
			width2 = width1 - width2;
			width1 -= width2;

			height1 += height2;
			height2 = height1 - height2;
			height1 -= height2;
		}

		// Calculate the 4 control points
		int xOffset = 0.25 * Ogre::Math::Abs(x1 - x2);
		xOffset = std::max(xOffset, xOffsetMin);
		if (x1 + width1 > x2 + width2)
		{
			x3 = x1 - xOffset;
			x4 = x2 - xOffset;
			if (x1 > x2 + width2)
			{
				x2 += width2;
				x4 = x2 + xOffset;
			}
		}
		else
		{
			x1 += width1;
			x3 = x1 + xOffset;
			x4 = x2 - xOffset;
			if (x1 > x2)
			{
				x2 += width2;
				x4 = x2 + xOffset;
			}
		}

		points[0] = wxPoint(x1, y1);
		points[1] = wxPoint(x3, y3);
		points[2] = wxPoint(x4, y4);
		points[3] = wxPoint(x2, y2);
		dc->DrawSpline(4, points);
	}
}
//-----------------------------------------------------------------------
bool DrawConnector::hasNode(wxWindow* node)
{
	return (node == mNode1 || node == mNode2);
}
//-----------------------------------------------------------------------
bool DrawConnector::hasNodesAndRelation(wxWindow* node1, wxWindow* node2, ComponentRelation relation)
{
	bool result;
	result = (node1 == mNode1 || node1 == mNode2);
	result = result && (node2 == mNode1 || node2 == mNode2);
	result = result && (relation == mRelation);
	return result;
}
//-----------------------------------------------------------------------
void DrawConnector::setPosition(wxPoint& position)
{
	mPosition = position;
}
//-----------------------------------------------------------------------
wxWindow* DrawConnector::getNode1 (void)
{
	return mNode1;
}
//-----------------------------------------------------------------------
wxWindow* DrawConnector::getNode2 (void)
{
	return mNode2;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ComponentRelation ConnectionPolicy::getRelation(void) const
{
	return mRelation;
}
//-----------------------------------------------------------------------
ComponentRelationDirection ConnectionPolicy::getRelationDirection(void) const
{
	return mRelationDirection;
}
//-----------------------------------------------------------------------
const wxString& ConnectionPolicy::getRelationDescription(ComponentRelation relation, 
	ComponentRelationDirection relationDirection, 
	const wxString& typeToBeConnectedWith, 
	const wxString& subTypeToBeConnectedWith) const
{
	if (relation == mRelation && relationDirection == mRelationDirection && typeToBeConnectedWith == mTypeToBeConnectedWith)
	{
		if (mIgnoreSubType)
		{
			return mRelationDescription;
		}
		else
		{
			if (subTypeToBeConnectedWith == mSubTypeToBeConnectedWith && subTypeToBeConnectedWith != CST_UNDEFINED)
			{
				return mRelationDescription;
			}
		}
	}
	return CRD_UNKNOWN;
}
//-----------------------------------------------------------------------
ComponentType ConnectionPolicy::getTypeToBeConnectedWith(void) const
{
	return mTypeToBeConnectedWith;
}
//-----------------------------------------------------------------------
ComponentSubType ConnectionPolicy::getSubTypeToBeConnectedWith(void) const
{
	return mSubTypeToBeConnectedWith;
}
//-----------------------------------------------------------------------
const Ogre::String& ConnectionPolicy::getColourCode(void) const
{
	return mColourCode;
}
//-----------------------------------------------------------------------
int ConnectionPolicy::getLineStyle(void) const
{
	return mLineStyle;
}
//-----------------------------------------------------------------------
bool ConnectionPolicy::isPolicyLocked(void) const
{
	return mPolicyLocked;
}
//-----------------------------------------------------------------------
bool ConnectionPolicy::areMultipleConnectionsPossible(void) const
{
	return mMultipleConnectionsPossible;
}
//-----------------------------------------------------------------------
bool ConnectionPolicy::ignoreSubType(void) const
{
	return mIgnoreSubType;
}
//-----------------------------------------------------------------------
const wxString& ConnectionPolicy::getRelationDescription(void) const
{
	return mRelationDescription;
}
//-----------------------------------------------------------------------
void ConnectionPolicy::validateAndLock(ComponentRelation relation, 
	ComponentRelationDirection relationDirection,
	ComponentType componentType, 
	ComponentSubType componentSubType, 
	bool lock)
{
	if (mMultipleConnectionsPossible)
		return;

	if (componentType == mTypeToBeConnectedWith && relation == mRelation  && relationDirection == mRelationDirection)
	{
		if (mIgnoreSubType)
		{
			mPolicyLocked = lock;
		}
		else
		{
			if (componentSubType == mSubTypeToBeConnectedWith && componentSubType != CST_UNDEFINED)
			{
				mPolicyLocked = lock;
			}
		}
	}
}
//-----------------------------------------------------------------------
void ConnectionPolicy::validateAndLock(ComponentRelation relation, 
	ComponentRelationDirection relationDirection,
	bool lock)
{
	if (relation == mRelation  && relationDirection == mRelationDirection)
	{
		mPolicyLocked = lock;
	}
}
//-----------------------------------------------------------------------
bool ConnectionPolicy::isConnectionPossible(
			ComponentType typeToBeConnectedWith, 
			ComponentSubType subTypeToBeConnectedWith)
{
	if (mIgnoreSubType)
	{
		return (typeToBeConnectedWith == mTypeToBeConnectedWith &&
			!mPolicyLocked);
	}
	else
	{
		if (typeToBeConnectedWith == mTypeToBeConnectedWith && !mPolicyLocked)
		{
			if (subTypeToBeConnectedWith == mSubTypeToBeConnectedWith && subTypeToBeConnectedWith != CST_UNDEFINED)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}
//-----------------------------------------------------------------------
bool ConnectionPolicy::isConnectionPossible(
			ComponentRelation relation,
			ComponentRelationDirection relationDirection,
			ComponentType typeToBeConnectedWith, 
			ComponentSubType subTypeToBeConnectedWith)
{
	return (isConnectionPossible(typeToBeConnectedWith, subTypeToBeConnectedWith) && relation == mRelation && relationDirection == mRelationDirection);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ComponentRelation Connection::getRelation(void) const
{
	return mRelation;
}
//-----------------------------------------------------------------------
ComponentRelationDirection Connection::getRelationDirection(void) const
{
	return mRelationDirection;
}
//-----------------------------------------------------------------------
EditComponent* Connection::getComponentToBeConnectedWith(void)
{
	return mComponentToBeConnectedWith;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
bool UniqueRelation::isRelationUnique(ComponentRelation relation, ComponentRelationDirection relationDirection)
{
	return (relation == mRelation && relationDirection == mRelationDirection);
}