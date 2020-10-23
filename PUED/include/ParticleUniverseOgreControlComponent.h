/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_OGRE_CONTROL_COMPONENT_H__
#define __PUED_OGRE_CONTROL_COMPONENT_H__

#include "wx/ogre/prerequisites.h"
#include "wx/ogre/control.h"

static const Ogre::String CURRENT_PS_NAME_SMALL = "currentParticleSystemNameSmall";

/**	DrawConnector class: Defines the connection between two windows.
*/
class OgreControlComponent : public wxOgreControl
{
	public:
	    /** Constructors. */    
		OgreControlComponent(void);
	    OgreControlComponent(wxWindow* parent, wxWindowID id = wxID_ANY,
		                    const wxPoint& pos = wxDefaultPosition,
			                const wxSize& size = wxDefaultSize,
							long style = wxSUNKEN_BORDER | wxTRANSPARENT_WINDOW,
			                const wxValidator& val = wxDefaultValidator,
			                const wxString& name = wxT("OgreControlComponent"));

		/**
			Various functions
		*/
		void createScene();
		//void notifyRescaled(ParticleUniverse::Real scale);
		void notifyTimeRescaled(ParticleUniverse::Real scale);

		/**
			Event handlers
		*/
		void OnWindowEnter(wxMouseEvent& event);
		void OnWindowLeave(wxMouseEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnMouseLButtonPressed(wxMouseEvent& event);
		void OnMouseRButtonPressed(wxMouseEvent& event);
		void OnClose(wxCommandEvent& event);

		// Funtions for the particle system that is showed
		ParticleUniverse::ParticleSystem* notifyCreateNewSystem(const Ogre::String& templateName);
		void notifyPlay(void);
		void notifyPause(void);
		void notifyStop(void);

		// Returns the created particle system (or 0)
		ParticleUniverse::ParticleSystem* getParticleSystem(void);

		// Returns the scenenode to which the particle system is attached
		Ogre::SceneNode* getMainSceneNode(void);

	protected:
		Ogre::SceneManager* mSceneManager;
		ParticleUniverse::ParticleSystem* mCurrentParticleSystem;
		Ogre::SceneNode* mMainSceneNode;
};

#endif

