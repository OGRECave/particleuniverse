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

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseOgreControlComponent.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditor.h"

/**
	This component is associated with the EditComponent for a particle system. Each change in the Edit-tab reflects
	to the particle system displayed by this component.
*/
//-----------------------------------------------------------------------
OgreControlComponent::OgreControlComponent(void) : 
	wxOgreControl()
{
}
//-----------------------------------------------------------------------
OgreControlComponent::OgreControlComponent(wxWindow* parent, 
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxValidator& val,
	const wxString& name) : 
	wxOgreControl(parent, id, pos, size, style, val, name),
	mSceneManager(0),
	mMainSceneNode(0),
	mCurrentParticleSystem(0)
{
	// Although some signal-slot construction should be used here, this also works. It is less elegant.
//	Connect(wxEVT_MOTION, wxMouseEventHandler(OgreControlComponent::OnMouseMove));
//	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(OgreControlComponent::OnWindowEnter));
//	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(OgreControlComponent::OnWindowLeave));
//	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(OgreControlComponent::OnMouseLButtonPressed));
//	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(OgreControlComponent::OnMouseRButtonPressed));
	Connect(wxEVT_DESTROY, wxCommandEventHandler(OgreControlComponent::OnClose));
	createScene();
}
//-----------------------------------------------------------------------
void OgreControlComponent::createScene()
{
    mSceneManager = CreateSceneManager(Ogre::ST_GENERIC);
	mMainSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	//Ogre::Vector3 position = Ogre::Vector3(0, 0, -1) + ParticleUniverseEditorFrame::getScale() * Ogre::Vector3(0, 0, -1100);
	Ogre::Vector3 position(0, 0, 0); 
	mMainSceneNode->setPosition(position); 
	if (m_cam)
	{
		m_cam->setNearClipDistance(0.0001f);
		m_cam->lookAt(position);
	}
    Refresh();
}
//-----------------------------------------------------------------------
//void OgreControlComponent::notifyRescaled(ParticleUniverse::Real scale)
//{
//	if (mMainSceneNode)
//	{
//		Ogre::Vector3 position = Ogre::Vector3(0, 0, -1) + scale * Ogre::Vector3(0, 0, -1100);
//		mMainSceneNode->setPosition(position);
//	}
//}
//-----------------------------------------------------------------------
void OgreControlComponent::notifyTimeRescaled(ParticleUniverse::Real scale)
{
	if (mCurrentParticleSystem)
	{
		mCurrentParticleSystem->setScaleTime(scale);
	}
}
//-----------------------------------------------------------------------
void OgreControlComponent::OnWindowEnter(wxMouseEvent& event)
{
	// Parent MUST be an Edit component
//	EditComponent* parent = static_cast<EditComponent*>(GetParent());
//	if (parent)
//	{
//		parent->OnWindowEnter(event);
//	}
}
//-----------------------------------------------------------------------
void OgreControlComponent::OnWindowLeave(wxMouseEvent& event)
{
	// Parent MUST be an Edit component
//	EditComponent* parent = static_cast<EditComponent*>(GetParent());
//	if (parent)
//	{
//		parent->OnWindowLeave(event);
//	}
}
//-----------------------------------------------------------------------
void OgreControlComponent::OnMouseMove(wxMouseEvent& event)
{
	// Parent MUST be an Edit component
//	EditComponent* parent = static_cast<EditComponent*>(GetParent());
//	if (parent)
//	{
//		parent->OnMouseMove(event);
//	}
}
//-----------------------------------------------------------------------
void OgreControlComponent::OnMouseLButtonPressed(wxMouseEvent& event)
{
	// Parent MUST be an Edit component
//	EditComponent* parent = static_cast<EditComponent*>(GetParent());
//	if (parent)
//	{
//		parent->OnMouseLButtonPressed(event);
//	}
}
//-----------------------------------------------------------------------
void OgreControlComponent::OnMouseRButtonPressed(wxMouseEvent& event)
{
	// Parent MUST be an Edit component
//	EditComponent* parent = static_cast<EditComponent*>(GetParent());
//	if (parent)
//	{
//		parent->OnMouseRButtonPressed(event);
//	}
}
//-----------------------------------------------------------------------
void OgreControlComponent::OnClose(wxCommandEvent& event)
{
	ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyParticleSystem(CURRENT_PS_NAME_SMALL, mSceneManager);
	ParticleUniverse::ParticleSystemManager::getSingletonPtr()->removeAndDestroyDanglingSceneNodes(mMainSceneNode);
}
//-----------------------------------------------------------------------
ParticleUniverse::ParticleSystem* OgreControlComponent::notifyCreateNewSystem(const Ogre::String& templateName)
{
	if (mCurrentParticleSystem)
	{
		mCurrentParticleSystem->stop();
		mMainSceneNode->detachObject(mCurrentParticleSystem);
		//mMainSceneNode->detachAllObjects();
	}

	try
	{
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyParticleSystem(CURRENT_PS_NAME_SMALL, mSceneManager);
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->removeAndDestroyDanglingSceneNodes(mMainSceneNode);
		mCurrentParticleSystem = 0;
		mCurrentParticleSystem = ParticleUniverse::ParticleSystemManager::getSingletonPtr()->createParticleSystem(
			CURRENT_PS_NAME_SMALL, templateName, mSceneManager);
		return mCurrentParticleSystem;
	}

	catch (Ogre::Exception)
	{
		Ogre::LogManager::getSingleton().logMessage("OgreControlComponent::notifyCreateNewSystem() Cannot create new system");
		if (mCurrentParticleSystem)
		{
			// Clean up the garbage (cannot be done by means of the ParticleSystemManager; apparently is doesn't exist in the ParticleSystemManager)
			delete mCurrentParticleSystem;
			mCurrentParticleSystem = 0;
		}
	}

	return 0;
}
//-----------------------------------------------------------------------
void OgreControlComponent::notifyPlay(void)
{
	if (mCurrentParticleSystem)
	{
		if (mCurrentParticleSystem->getState() == ParticleUniverse::ParticleSystem::PSS_PAUSED)
		{
			mCurrentParticleSystem->resume();
		}
		else
		{
			if (mMainSceneNode->numAttachedObjects() == 0)
			{
				mMainSceneNode->attachObject(mCurrentParticleSystem);
			}
			mCurrentParticleSystem->start();
		}
	}
}
//-----------------------------------------------------------------------
void OgreControlComponent::notifyPause(void)
{
	if (mCurrentParticleSystem)
	{
		mCurrentParticleSystem->pause();
	}
}
//-----------------------------------------------------------------------
void OgreControlComponent::notifyStop(void)
{
	if (mCurrentParticleSystem)
	{
		mCurrentParticleSystem->stop();
	}
}
//-----------------------------------------------------------------------
ParticleUniverse::ParticleSystem* OgreControlComponent::getParticleSystem(void)
{
	return mCurrentParticleSystem;
}
//-----------------------------------------------------------------------
Ogre::SceneNode* OgreControlComponent::getMainSceneNode(void)
{
	return mMainSceneNode;
}
