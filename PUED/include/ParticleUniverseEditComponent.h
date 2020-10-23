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

#ifndef __PUED_EDIT_COMPONENT_H__
#define __PUED_EDIT_COMPONENT_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseConnector.h"
#include "ParticleUniversePropertyWindow.h"
#include "EmitterPropertyWindow/EmitterPropertyWindowFactory.h"
#include "AffectorPropertyWindow/AffectorPropertyWindowFactory.h"
#include "RendererPropertyWindow/RendererPropertyWindowFactory.h"
#include "ObserverPropertyWindow/ObserverPropertyWindowFactory.h"
#include "BehaviourPropertyWindow/BehaviourPropertyWindowFactory.h"
#include "EventHandlerPropertyWindow/EventHandlerPropertyWindowFactory.h"
#include "ExternPropertyWindow/ExternPropertyWindowFactory.h"

class ParticleUniverseEditorFrame;

/**	Edit Component: This defines the modules on the EditCanvas.
*/
class EditComponent : public wxMDIChildFrame
{
	public:
		// Constructor / Destructor
		EditComponent(wxMDIParentFrame* parent, 
			const Ogre::String& name, 
			ComponentType type, 
			ComponentSubType subType, 
			const wxColour& backgroundColor = wxNullColour,
			wxSize size = wxSize(200, 80),
			long style = wxSYSTEM_MENU | wxCAPTION);
		~EditComponent(void);
		ComponentType getComponentType(void) const;
		ComponentSubType getComponentSubType(void) const;

		/**	Add a connection policy. These are the possible policies
		*/
		void addPolicy(
			ComponentRelation relation,
			ComponentRelationDirection relationDirection,
			const wxString& relationDescription,
			ComponentType typeToBeConnectedWith, 
			ComponentSubType subTypeToBeConnectedWith = CST_UNDEFINED,
			bool multipleConnectionsPossible = true,
			bool ignoreSubType = true,
			const Ogre::String& colourCode = DRAW_DEFAULT_COLOURCODE,
			int lineStyle = wxSOLID);

		/**	Make a relation unique. This is an addition to the policies. Sometimes, a relation can be used in combination with 
			several component types, but only one relation is allowed.
		*/
		void addUniqueRelation(ComponentRelation relation, ComponentRelationDirection relationDirection);

		/**	Add a connection. These are the actual connections with other components
		*/
		void addConnection(EditComponent* componentToBeConnectedWith, 
			ComponentRelation relation, 
			ComponentRelationDirection relationDirection);

		/**	Returns a connection policy
		*/
		ConnectionPolicy* getPolicy(ComponentRelation relation,  
			ComponentRelationDirection relationDirection, 
			ComponentType typeToBeConnectedWith, 
			ComponentSubType subTypeToBeConnectedWith = CST_UNDEFINED);

		/**	Checks whether a relation is unique for this component
		*/
		bool isRelationUnique(ComponentRelation relation, ComponentRelationDirection relationDirection);

		/**	Checks whether a component can be connected at all (to another component)
		*/
		bool isConnectionPossible(void);

		/**	Checks whether a component can be connected to another component.
			This function validates its own connection policies and determines for each possible policy whether the component - passed as an
			argument - can be connected using the attributes of that policy.
		*/
		bool isConnectionPossible(EditComponent* component);

		/**	Checks whether this component can be connected.
			All registered policies are checked against the passed arguments.
		*/
		bool isConnectionPossible(ComponentRelation relation, 
			ComponentRelationDirection relationDirection,
			ComponentType typeToBeConnectedWith, 
			ComponentSubType subTypeToBeConnectedWith);

		/**	Determines whether it is possible to disconnect a connection.
		*/
		bool isDisconnectionPossible(void);

		/**	Determines whether it is possible to disconnect a connection.
		*/
		bool isConnected(EditComponent* componentToBeConnectedWith, ComponentRelation relation, ComponentRelationDirection relationDirection);

		/**	Displays a selection dialog with all possible connections (for a component to which it must be connected)
		*/
		ConnectionPolicy* selectPolicy(EditComponent* componentToBeConnectedWith);

		/**	Displays a selection dialog with all connections. If viewOnly is set to false, it is also possible to delete a connection.
		*/
		void selectConnection(bool viewOnly = true);

		/**	Returns the selected policy
		*/
		ConnectionPolicy* getSelectedPolicy(void);

		/**	Deletes a Connection if the arguments match (and also take care of the connected component)
		*/
		void deleteConnection(EditComponent* componentConnectedWith, 
			ComponentRelation relation, 
			ComponentRelationDirection relationDirection);

		/**	Deletes a Connection (and also take care of the connected component)
		*/
		void deleteConnection(Connection* connection);

		/**	Deletes all connections of a certain relation and direction (and also take care of the connected component)
		*/
		void deleteConnection(ComponentRelation relation, ComponentRelationDirection relationDirection);

		/**	Unlocks a policy based on the arguments
		*/
		void unlockPolicy(ComponentRelation relation, 
			ComponentRelationDirection relationDirection,
			ComponentType typeToBeConnectedWith, 
			ComponentSubType subTypeToBeConnectedWith);

		/**	Returns a description of the relation with this direction, type and subtype
		*/
		const wxString& getRelationDescription(ComponentRelation relation, 
			ComponentRelationDirection relationDirection, 
			ComponentType typeToBeConnectedWith, 
			ComponentSubType subTypeToBeConnectedWith);

		/**	Set the name on the header, with the class attributes
		*/
		void setCaption(void);

		/**	Getters/Setters
		*/
		const ComponentSubType& getSubType(void) const;
		void setSubType(ComponentSubType subType);
		const Ogre::String& getComponentName(void) const;
		void setComponentName(const Ogre::String& componentName);

		/**	Refreshes the canvas
		*/
		void refreshCanvas(void);

		/**	Create a property window and use the argument for propagation
		*/
		PropertyWindow* createPropertyWindow(ComponentSubType subType, const PropertyWindow* propertyWindow = 0);

		/**	Returns the property window
		*/
		PropertyWindow* getPropertyWindow(void);

		/**	Notify the parent if a property has been changed
		*/
		void notifyPropertyChanged(void);

		/**	Notify the parent if the name of a component has been changed
		*/
		void notifyAdjustNames(const Ogre::String& newName);

		/**	Handle events
		*/
		void OnMove(wxMoveEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnWindowEnter(wxMouseEvent& event);
		void OnWindowLeave(wxMouseEvent& event);
		void OnMouseLButtonPressed(wxMouseEvent& event);
		void OnMouseRButtonPressed(wxMouseEvent& event);
		void OnActivate(wxActivateEvent& event);

		/**	Get/Set PU element
		*/
		ParticleUniverse::IElement* getPUElement(void) {return mPUElement;};
		void setPUElement(ParticleUniverse::IElement* puElement){mPUElement = puElement;};

		/**	Returns the size at the moment the component was created
		*/
		wxSize getOriginalSize() {return mOriginalSize;};

		/**
			Get/Set root frame
		*/
		ParticleUniverseEditorFrame* getRootFrame(void);
		void setRootFrame(ParticleUniverseEditorFrame* rootFrame);

	protected:
		ParticleUniverse::IElement* mPUElement;
		wxWindow* mRootParent;
		Ogre::String mName;
		Ogre::String mParentName;
		ComponentType mType;
		ComponentSubType mSubType;
		std::vector<ConnectionPolicy*> mPolicies;
		std::vector<Connection*> mConnections;
		std::vector<UniqueRelation*> mUniqueRelations;
		ConnectionPolicy* mSelectedPolicy;
		PropertyWindow* mPropertyWindow;
		PropertyWindow* mOldPropertyWindow;
		EmitterPropertyWindowFactory mEmitterPropertyWindowFactory;
		RendererPropertyWindowFactory mRendererPropertyWindowFactory;
		AffectorPropertyWindowFactory mAffectorPropertyWindowFactory;
		ObserverPropertyWindowFactory mObserverPropertyWindowFactory;
		BehaviourPropertyWindowFactory mBehaviourPropertyWindowFactory;
		EventHandlerPropertyWindowFactory mEventHandlerPropertyWindowFactory;
		ExternPropertyWindowFactory mExternPropertyWindowFactory;
		wxSize mOriginalSize;

		/**	If the edit component is closed, connections with other components must be deleted in a certain order.
		*/
		void _sortConnections(void);

		/**
			Keep pointer to the root frame
		*/
		ParticleUniverseEditorFrame* mRootFrame;

	private:
		EditComponent(void){};
};

#endif
