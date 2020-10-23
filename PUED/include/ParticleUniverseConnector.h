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

#ifndef __PUED_CONNECTOR_H__
#define __PUED_CONNECTOR_H__

#include "wx/ogre/prerequisites.h"

class EditComponent;

/**	DrawConnector class: Defines the connection between two windows.
*/
class DrawConnector : public wxFrame
{
	public:
		DrawConnector(wxWindow* node1, 
			wxWindow* node2, 
			ComponentRelation relation, 
			const Ogre::String& colourCode = DRAW_DEFAULT_COLOURCODE, 
			int lineStyle = wxSOLID);
		DrawConnector(wxWindow* node, 
			wxPoint& position, 
			const Ogre::String& colourCode = DRAW_DEFAULT_COLOURCODE, 
			int lineStyle = wxSOLID);
		~DrawConnector(void){};
		void drawLine(wxWindow* canvas, wxPaintDC* dc);
		bool hasNode(wxWindow* node);
		bool hasNodesAndRelation(wxWindow* node1, wxWindow* node2, ComponentRelation relation);
		void setPosition(wxPoint& position);
		wxWindow* getNode1 (void);
		wxWindow* getNode2 (void);

	protected:
		wxWindow* mNode1;
		wxWindow* mNode2;
		wxPoint mPosition;
		bool usePosition;
		Ogre::String mColourCode;
		int mLineStyle;
		ComponentRelation mRelation;
		int xOffsetMin;
};


/**	ConnectionPolicy: Defines the possible relations between 2 component types. Example:
	A 'technique' component may include an 'emitter' component. A ConnectionPolicy for the 'emitter' component
	is defined by:
	emitter included by technique

	Also subtypes can be used:
	affector handled by handler:do affector

*/
class ConnectionPolicy
{
	public:
		ConnectionPolicy(
			ComponentRelation relation,
			ComponentRelationDirection relationDirection,
			wxString relationDescription,
			ComponentType typeToBeConnectedWith, 
			ComponentSubType subTypeToBeConnectedWith = CST_UNDEFINED,
			bool multipleConnectionsPossible = false,
			bool ignoreSubType = true,
			Ogre::String colourCode = DRAW_DEFAULT_COLOURCODE,
			int lineStyle = wxSOLID) : 
			
			mRelation(relation),
			mRelationDirection(relationDirection),
			mRelationDescription(relationDescription),
			mTypeToBeConnectedWith(typeToBeConnectedWith),
			mSubTypeToBeConnectedWith(subTypeToBeConnectedWith),
			mMultipleConnectionsPossible(multipleConnectionsPossible),
			mColourCode(colourCode),
			mLineStyle(lineStyle),
			mPolicyLocked(false),
			mIgnoreSubType(ignoreSubType){};
		
		~ConnectionPolicy(void){};

		// Getters
		ComponentRelation getRelation(void) const;
		ComponentRelationDirection getRelationDirection(void) const;
		const wxString& ConnectionPolicy::getRelationDescription(ComponentRelation relation, 
			ComponentRelationDirection relationDirection,
			const wxString& typeToBeConnectedWith, 
			const wxString& subTypeToBeConnectedWith) const;
		const wxString& getRelationDescription(void) const;
		ComponentType getTypeToBeConnectedWith(void) const;
		ComponentSubType getSubTypeToBeConnectedWith(void) const;
		const Ogre::String& getColourCode(void) const;
		int getLineStyle(void) const;
		bool isPolicyLocked(void) const;
		bool areMultipleConnectionsPossible(void) const;
		bool ignoreSubType(void) const;

		/**	Determines if a connection with this type of component possible
		*/
		bool isConnectionPossible(
			ComponentType typeToBeConnectedWith, 
			ComponentSubType subTypeToBeConnectedWith = CST_UNDEFINED);

		/**	This function not only determines whether a connection is possible, but also if a specific connection is possible
		*/
		bool isConnectionPossible(
			ComponentRelation relation,
			ComponentRelationDirection relationDirection,
			ComponentType typeToBeConnectedWith, 
			ComponentSubType subTypeToBeConnectedWith = CST_UNDEFINED);

		/**	This function determines the impact on the policy and locks or unlocks if necessary
		*/
		void validateAndLock(ComponentRelation relation, 
			ComponentRelationDirection relationDirection,
			ComponentType componentType, 
			ComponentSubType componentSubType, 
			bool lock = true);

		/**	This function locks or unlocks a policy if it has a specific relation
		*/
		void validateAndLock(ComponentRelation relation, 
			ComponentRelationDirection relationDirection,
			bool lock = true);

	protected:
		ComponentRelation mRelation;
		ComponentRelationDirection mRelationDirection;
		wxString mRelationDescription;
		ComponentType mTypeToBeConnectedWith;
		ComponentSubType mSubTypeToBeConnectedWith;
		bool mMultipleConnectionsPossible;
		bool mPolicyLocked;
		bool mIgnoreSubType;
		Ogre::String mColourCode;
		int mLineStyle;
};


/**	Connection: Defines the actual relation between the component and another component.
*/
class Connection
{
	public:
		Connection(EditComponent* componentToBeConnectedWith, ComponentRelation relation, ComponentRelationDirection relationDirection) : 
			mComponentToBeConnectedWith(componentToBeConnectedWith),
			mRelation(relation),
			mRelationDirection(relationDirection){};
		~Connection(void){};

		// Getter
		EditComponent* getComponentToBeConnectedWith(void);
		ComponentRelation getRelation(void) const;
		ComponentRelationDirection getRelationDirection(void) const;

	protected:
		EditComponent* mComponentToBeConnectedWith;
		ComponentRelation mRelation;
		ComponentRelationDirection mRelationDirection;
};

/**	For storage of unique combinations
*/
class UniqueRelation
{
	public:
		UniqueRelation(ComponentRelation relation, ComponentRelationDirection relationDirection) : 
			mRelation(relation),
			mRelationDirection(relationDirection){};
		~UniqueRelation(void){};

		bool isRelationUnique(ComponentRelation relation, ComponentRelationDirection relationDirection);
	
	protected:
		ComponentRelation mRelation;
		ComponentRelationDirection mRelationDirection;
};

#endif

