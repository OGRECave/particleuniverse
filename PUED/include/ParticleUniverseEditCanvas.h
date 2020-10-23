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

#ifndef __PUED_EDIT_CANVAS_H__
#define __PUED_EDIT_CANVAS_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseConnector.h"
#include "ParticleUniverseEditComponent.h"

/**	Draw surface: The background canvas on which the EditComponents are placed. The EditCanvas is used to draw the lines.
*/
class EditCanvas : public wxMDIChildFrame
{
	public:
		enum SelectionMode
		{
			SM_SELECT_NONE,
			SM_SELECTING,
			SM_SELECTED,
			SM_MOVING
		};

		// Constructor / Destructor
		EditCanvas(wxMDIParentFrame* parent);
		~EditCanvas(void){};

		/**	Handlers
		*/
		virtual void OnPaint(wxPaintEvent& event);
		void OnActivate(wxActivateEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnMouseLeave(wxMouseEvent& event);
		void OnMouseLButtonPressed(wxMouseEvent& event);
		void OnMouseLButtonReleased(wxMouseEvent& event);
		void OnMouseRButtonPressed(wxMouseEvent& event);
		void OnKeyPressed(wxKeyEvent& event);
		void OnMouseWheel(wxMouseEvent& event);

		/**	Connect two components
		*/
		void connect(EditComponent* node1, 
			EditComponent* node2, 
			ComponentRelation relation, 
			const Ogre::String colourCode = DRAW_DEFAULT_COLOURCODE,
			int lineStyle = wxSOLID);

		/**	Return all connections between windows
		*/
		std::vector<DrawConnector*>& getDrawConnections(void);

		/**	Return the number of connected windows
		*/
		unsigned int getNumberOfConnections(void) const;

		/**	Remove connection: This removes all connections of this component
		*/
		void removeConnection(EditComponent* node);

		/**	Remove connection between 2 nodes with a specific relation
		*/
		void removeConnection(EditComponent* node1, EditComponent* node2, ComponentRelation relation);

		/**	Create a Connector between a node and the mouse
		*/
		void createMouseConnector(wxWindow* node);

		/**	Destroy the Connector between the node and the mouse
		*/
		void destroyMouseConnector(void);

		/**	Store the selected components in a list
		*/
		void selectComponents(void);

		/**	Move the the selected components in the list
		*/
		void moveSelectedComponents(const wxPoint& previousMousePosition, const wxPoint& mMousePosition);

		/**	Delete the selected components
		*/
		void deleteSelectedComponents(void);

		/**	Adjustment the mousePosition based on mouse movement in MDI Child windows
		*/
		void adjustMousePosition(const wxPoint& componentPosition,  const wxPoint& mousePosition);

	protected:
		std::vector<DrawConnector*> mDrawConnectors;
		DrawConnector* mMouseConnector;
		wxPoint mMousePosition;
		wxPoint mMousePositionAdjustment;
		SelectionMode mSelectionMode;
		wxPoint mStartPositionSelection;
		wxPoint mEndPositionSelection;
		std::vector<EditComponent*> mSelectedComponents;

	private:
		EditCanvas(void){};
};

#endif
