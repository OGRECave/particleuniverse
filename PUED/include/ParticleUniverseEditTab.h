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

#ifndef __PUED_EDIT_TAB_H__
#define __PUED_EDIT_TAB_H__

#include "wx/ogre/prerequisites.h"
#include "wx/notebook.h"
#include "ParticleUniverseEditToolbar.h"
#include "ParticleUniverseEditCanvas.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseOgreControlComponent.h"
#include "ParticleUniverseSystemPropertyWindow.h"

// Enums: Component + Relation Descriptions
static ComponentType CT_SYSTEM = wxT("");
static ComponentType CT_TECHNIQUE = wxT("");
static ComponentType CT_RENDERER = wxT("");
static ComponentType CT_EMITTER = wxT("");
static ComponentType CT_AFFECTOR = wxT("");
static ComponentType CT_OBSERVER = wxT("");
static ComponentType CT_HANDLER = wxT("");
static ComponentType CT_BEHAVIOUR = wxT("");
static ComponentType CT_EXTERN = wxT("");
static ComponentType CT_VISUAL = wxT("");

static wxString CRD_INCLUDES = wxT("");
static wxString CRD_INCLUDED_BY = wxT("");
static wxString CRD_EXCLUDES = wxT("");
static wxString CRD_EXCLUDED_BY = wxT("");
static wxString CRD_HANDLES = wxT("");
static wxString CRD_HANDLED_BY = wxT("");
static wxString CRD_EMITS = wxT("");
static wxString CRD_EMITTED_BY = wxT("");
static wxString CRD_AFFECTS = wxT("");
static wxString CRD_AFFECTED_BY = wxT("");
static wxString CRD_OBSERVES = wxT("");
static wxString CRD_OBSERVED_BY = wxT("");
static wxString CRD_INTERFACES = wxT("");
static wxString CRD_INTERFACED_BY = wxT("");
static wxString CRD_SLAVES = wxT("");
static wxString CRD_SLAVE_OF = wxT("");
static wxString CRD_MASTER = wxT("");
static wxString CRD_ENABLES = wxT("");
static wxString CRD_ENABLED_BY = wxT("");
static wxString CRD_FORCES = wxT("");
static wxString CRD_FORCED_BY = wxT("");
static wxString CRD_PLACES = wxT("");
static wxString CRD_PLACED_BY = wxT("");
static wxString CRD_UNKNOWN = wxT("");

class EditTools;

/**	Client Window
*/
class ClientWindow : public wxMDIClientWindow
{
	public:
		// Constructor / Destructor
		ClientWindow(wxMDIParentFrame* parent);
		~ClientWindow(void){};

	protected:

	private:
		ClientWindow(void){};
};


/**	Edit Tab: Class that defines the actual notebook tab.
*/
class EditTab : public wxMDIParentFrame
{
	public:
		enum ConnectionMode
		{
			CM_CONNECT_NONE,
			CM_CONNECT_STARTING,
			CM_CONNECT_ENDING,
			CM_DISCONNECT
		};

		// Enum used to trigger some kind of action
		enum SimpleEvent
		{
			SE_CLOSE,
			SE_NAME_CHANGED
		};

		// Constructor / Destructor
		EditTab(wxWindow* parentNotebook, wxWindow* rootParent);
		~EditTab(void);

		/**	Reposition the MDI frame according to its parent
		*/
		void adjustPosition(void);

		/**	Returns the number of systems on the edit canvas
		*/
		unsigned int getNumberOfSystems() const;
		
		/**	Returns the number of components on the edit canvas
		*/
		unsigned int getNumberOfComponents() const;

		/**	Push a component to the stack
		*/
		void pushSystem(EditComponent* system);

		/**	Push a component on the stack
		*/
		void pushComponent(EditComponent* component);

		/**	Pop a component from the stack
		*/
		void popComponent(EditComponent* component);

		/**	Update if the MDI Child windows have been changed
		*/
		void refreshCanvas(void);

		/**	Set the focus to the canvas again to be able to select the MDI Childs
		*/
		void notifyFocusLeft(void);

		/**	Perform actions (if needed) if one of the components is activated
		*/
		void notifyComponentActivated(EditComponent* component);

		/**	Remove an MDI Child component, so update the canvas
		*/
		void notifyComponentRemoved(EditComponent* component);

		/**	Something has probably changed regarding the connections.
		*/
		void notifyConnectionsChanged();

		/**	If an event occurs on the EditComponent, some kind of action can be performed.
		@remarks
			This function is used for other EditComponents to act on a situation one the EditComponent that is passed as an argument.
		*/
		void notifyReferers(EditComponent* component, SimpleEvent simpleEvent);

		/**	Connection between 2 components is interrupted
		*/
		void resetConnectionMode(void);

		/**	A connection between 2 components is removed
		*/
		void notifyConnectionRemoved(EditComponent* node1, 
			EditComponent* node2, 
			ComponentRelation relation,
			ComponentRelationDirection relationDirection);

		/**	A connection between 2 components is established
		*/
		void notifyConnectionAdded(EditComponent* node1, 
			EditComponent* node2, 
			ComponentRelation relation,
			ComponentRelationDirection relationDirection);

		/**	Returns a pointer to the canvas
		*/
		EditCanvas* getEditCanvas(void) const;

		/**	Get and Set the connectionmode. This is used for connecting components
		*/
		ConnectionMode getConnectionMode(void) const;
		void setConnectionMode(ConnectionMode connectionMode);

		/**	Return the list with all components
		*/
		std::vector<EditComponent*>& getComponents(void);

		/**	The mouse has moved in the component. This is important for the canvas.
		*/
		void notifyMouseMovedInComponent(const EditComponent* component, const wxPoint& mousePosition);

		/**	Checks whether a component can be connected to the component that is already connected to the mouse
		*/
		bool isConnectionPossible(EditComponent* component);

		/**	Returns the start connector (used when connecting with the mouse)
		*/
		EditComponent* getStartConnector(void);

		/**	Set a property window
		*/
		void setPropertyWindow(PropertyWindow* propertyWindow);

		/**	Removes a property window
		*/
		void removePropertyWindow(wxPropertyGrid* propertyWindow);

		/**	Set of functions to create one particle universe component (of a specific type).
		@remarks
			These functions are used to create a default particle universe component and sets it in the EditComponent.
		*/
		void createTechniqueForComponent(EditComponent* component);
		void createRendererForComponent(const Ogre::String& type, EditComponent* component);
		void createEmitterForComponent(const Ogre::String& type, EditComponent* component);
		void createAffectorForComponent(const Ogre::String& type, EditComponent* component);
		void createObserverForComponent(const Ogre::String& type, EditComponent* component);
		void createHandlerForComponent(const Ogre::String& type, EditComponent* component);
		void createBehaviourForComponent(const Ogre::String& type, EditComponent* component);
		void createExternForComponent(const Ogre::String& type, EditComponent* component);

		/**	Set of functions to destroy one particle universe component (of a specific type).
		@remarks
			These functions are used to destroy a particle universe component, remove it from the EditComponent and update the ParticleSystem 
			structure is needed.
		*/
		void destroyTechniqueFromComponent(EditComponent* component);
		void destroyRendererFromComponent(EditComponent* component);
		void destroyEmitterFromComponent(EditComponent* component);
		void destroyAffectorFromComponent(EditComponent* component);
		void destroyObserverFromComponent(EditComponent* component);
		void destroyHandlerFromComponent(EditComponent* component);
		void destroyBehaviourFromComponent(EditComponent* component);
		void destroyExternFromComponent(EditComponent* component);

		/**	Set of functions to create one edit component of a specific type.
		@remarks
			The default ones are the first one of the list. If the order of the list changes, make sure the defaults below also change.
		*/
		EditComponent* createParticleSystemEditComponent(void);
		EditComponent* createTechniqueEditComponent();
		EditComponent* createRendererEditComponent(const wxString& type);
		EditComponent* createEmitterEditComponent(const wxString& type);
		EditComponent* createAffectorEditComponent(const wxString& type);
		EditComponent* createObserverEditComponent(const wxString& type);
		EditComponent* createHandlerEditComponent(const wxString& type);
		EditComponent* createBehaviourEditComponent(const wxString& type);
		EditComponent* createExternEditComponent(const wxString& type);

		/**	Delete and Create all edit components, including property windows for the edit page at once, based on a template name.
			These functions are typically called when another template script is selected or when a new template script is 
			created.
		*/
		void deleteParticleSystemComponents(void);
		EditComponent* forceCreateParticleSystemEditComponent(void);
		bool copyParticleSystemPropertiesToPropertyWindow(EditComponent* particleSystemEditComponent, ParticleUniverse::ParticleSystem* particleSystem);
		bool createParticleSystemComponents(EditComponent* particleSystemEditComponent, ParticleUniverse::ParticleSystem* particleSystem);

		/**	Clear all particle universe components that are not part of a particle system.
		*/
		void destroyDanglingPUComponents(void);

		/**	Create all edit components, including property windows for the edit page at once, based on a template name.
			This function is typically called when another template script is selected or when a new template script is 
			created.
		*/
		wxPoint createComponentsFromTechnique(EditComponent* systemEditComponent, 
			ParticleUniverse::ParticleTechnique* technique,
			wxPoint position);

		/**	Create renderer edit component from a certain renderer.
		*/
		void createComponentFromRenderer(EditComponent* techniqueEditComponent, 
			ParticleUniverse::ParticleRenderer* renderer,
			wxPoint position);

		/**	Create emitter edit component from a certain emitter.
		*/
		void createComponentFromEmitter(EditComponent* techniqueEditComponent, 
			ParticleUniverse::ParticleEmitter* emitter,
			wxPoint position);

		/**	Create affector edit component from a certain emitter.
		*/
		void createComponentFromAffector(EditComponent* techniqueEditComponent, 
			ParticleUniverse::ParticleAffector* affector,
			wxPoint position);

		/**	Create observer edit component from a certain observer.
		*/
		int createComponentFromObserver(EditComponent* techniqueEditComponent, 
			ParticleUniverse::ParticleObserver* observer,
			wxPoint position,
			int latestHandlerY);
		
		/**	Create event handler edit component from a certain event handler.
		*/
		void createComponentFromEventHandler(EditComponent* observerEditComponent, 
			ParticleUniverse::ParticleEventHandler* eventHandler,
			wxPoint position);

		/**	Create behaviour edit component from a certain behaviour.
		*/
		void createComponentFromBehaviour(EditComponent* techniqueEditComponent, 
			ParticleUniverse::ParticleBehaviour* behaviour,
			wxPoint position);
	
		/**	Create extern edit component from a certain extern object.
		*/
		void createComponentFromExtern(EditComponent* techniqueEditComponent, 
			ParticleUniverse::Extern* externObject,
			wxPoint position);
	
		/**	Establish all connections other than CR_INCLUDE
		*/
		void createOtherConnections(const ParticleUniverse::ParticleTechnique* technique);

		/**	Establish a connection between 2 edit components
		*/
		void createConnection(EditComponent* componentPrimary, 
			EditComponent* componentSecundairy, 
			ComponentRelation relation, 
			ComponentRelationDirection direction);

		/**	Search for a component
		*/
		EditComponent* getParticleSystemEditComponent(void);
		EditComponent* findEditComponent(const wxString& name, const ComponentType& type, EditComponent* skip = 0);
		EditComponent* findEditComponent(const ParticleUniverse::IElement* puElement);
		EditComponent* findEditComponentForTechnique(const wxString& name, const wxString& techniqueName);

		/**	Scale all components.
		*/
		void scaleEditComponents(ParticleUniverse::Real scaleFactor);

		/**	Event handler for scaling.
		*/
		void OnMouseWheel(wxMouseEvent& event);
		void OnKeyPressed(wxKeyEvent& event);

		/**	If the particle system was updated because of some change on the edit tab, this function returns true.
		*/
		bool isSystemUpdatedByEditPage(void);

		/**	Set the mEditChanged attribute.
		*/
		void setSystemUpdatedByEditPage(bool updated = true) {mEditChanged = updated;};

		/**	Returns the created Particle System (or 0).
		*/
		ParticleUniverse::ParticleSystem* getParticleSystemFromSystemComponent(void);

		/**	Returns the SceneManager with which the CurrentParticleSystemForEditComponent was created.
		*/
//		Ogre::SceneManager* getSceneManager(void);

		/** Functions to stop and restart the particle system if needed
		*/
		bool _mustStopParticleSystem(void);
		void _mustRestartParticleSystem(bool wasStarted);

		/**
			Generate a name and set it in the PUElemen, the EditComponent and the Propertywindow
		*/
		void _generateNameForComponentAndPUElement(EditComponent* component, ComponentType type);

		/**
			If a name has been changed, the references of all particle system components must change also
		*/
		void adjustNames(const Ogre::String& oldName, const Ogre::String& newName, ComponentType type);

		/**
			Enable and disable tools
		*/
		void enableTools(bool enable);

		void FreezeClientWindow();
		void ThawClientWindow();

	protected:
		unsigned int mSystemCounter;
		unsigned int mTechniqueCounter;
		unsigned int mEmitterCounter;
		unsigned int mRendererCounter;
		unsigned int mAffectorCounter;
		unsigned int mObserverCounter;
		unsigned int mHandlerCounter;
		unsigned int mBehaviourCounter;
		unsigned int mExternCounter;
		int mOffsetX; // Used to dynamically arrange the edit components after loading a new particle system
		int mOffsetY; // ,,
		ParticleUniverse::Real mOffsetFraction;
		ParticleUniverse::Real mScale;

		wxWindow* mRootParent;
		ClientWindow* mClientWindow;
		EditTools* mEditTools;
		std::vector<EditComponent*> mComponents;
		unsigned int mNumberOfSystems;
		EditCanvas* mCanvas;
		ConnectionMode mConnectionMode;
		EditComponent* mStartConnector;
		EditComponent* mEndConnector;
		bool mEditChanged; // Determines whether the particle system was changed in the edit tab.

		/**	Remove all components, but don't destroy them.
		*/
		void _removeComponentsFromTechnique(ParticleUniverse::ParticleTechnique* technique);

		/**	Functions used when a certain connection has been established.
		*/
		bool _processIncludeAdded(EditComponent* node1, EditComponent* node2);
		bool _processExcludeAdded(EditComponent* node1, EditComponent* node2);
		bool _processEmitAdded(EditComponent* node1, EditComponent* node2, ComponentRelationDirection relationDirection);
		bool _processInterfaceAdded(EditComponent* node1, EditComponent* node2);
		bool _processSlaveAdded(EditComponent* node1, EditComponent* node2);
		bool _processEnableAdded(EditComponent* node1, EditComponent* node2);
		bool _processForceAdded(EditComponent* node1, EditComponent* node2);
		bool _processPlaceAdded(EditComponent* node1, EditComponent* node2);

		/**	Functions used when a certain connection has been deleted.
		*/
		bool _processIncludeRemoved(EditComponent* node1, EditComponent* node2);
		bool _processExcludeRemoved(EditComponent* node1, EditComponent* node2);
		bool _processEmitRemoved(EditComponent* node1, EditComponent* node2);
		bool _processInterfaceRemoved(EditComponent* node1, EditComponent* node2);
		bool _processSlaveRemoved(EditComponent* node1, EditComponent* node2);
		bool _processEnableRemoved(EditComponent* node1, EditComponent* node2);
		bool _processForceRemoved(EditComponent* node1, EditComponent* node2);
		bool _processPlaceRemoved(EditComponent* node1, EditComponent* node2);

	private:
		EditTab(void){};
};

/**	Edit Tools: The toolbar on the edit window.
*/
class EditTools : public wxDialog
{
	public:
		// Constructor / Destructor
		EditTools(EditTab* parent);
		~EditTools(void){};

		/**	Event Handlers
		*/
		//void OnNewSystem(wxCommandEvent& event);
		void OnNewTechnique(wxCommandEvent& event);
		void OnNewRenderer(wxCommandEvent& event);
		void OnNewEmitter(wxCommandEvent& event);
		void OnNewAffector(wxCommandEvent& event);
		void OnNewObserver(wxCommandEvent& event);
		void OnNewHandler(wxCommandEvent& event);
		void OnNewBehaviour(wxCommandEvent& event);
		void OnNewExtern(wxCommandEvent& event);
		void notifyDeleteSystem(void);
		void notifyDeleteTechnique(void);
		void notifyDeleteRenderer(void);
		void notifyDeleteEmitter(void);
		void notifyDeleteAffector(void);
		void notifyDeleteObserver(void);
		void notifyDeleteHandler(void);
		void notifyDeleteBehaviour(void);
		void notifyDeleteExtern(void);
		void OnConnect(wxCommandEvent& event);
		void OnDisconnect(wxCommandEvent& event);
		void OnHelp(wxCommandEvent& event);
		void OnCursor(wxCommandEvent& event);
		void notifyConnectionsChanged(void);

	protected:
		UIEditToolbar* mEditToolbar;
		EditTab* mParent;

		/**	Set the icons based on the current situation
		*/
		void resetIcons(void);

	private:
		EditTools(void){};
		virtual bool Destroy();
};

#endif
