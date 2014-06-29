#ifndef __ParticleUniverseDemo_H__
#define __ParticleUniverseDemo_H__

#include "ParticleUniverseSystemManager.h"

#include "BaseApplication.h"

#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneNode.h>

using namespace Ogre;

class Sample_ParticleUniverseDemo : public BaseApplication
{
protected:
	Ogre::SceneManager* mSceneMgr;
	Ogre::Viewport* mViewport;
	Ogre::Camera* mCamera;

/** -----------------------------------------------------------------------
	Create some particle systems
	-----------------------------------------------------------------------*/
	void createScene()
	{		
		mSceneMgr = Root::getSingleton().createSceneManager(ST_GENERIC);
		mCamera = mSceneMgr->createCamera("Camera");
		mCamera->setPosition(320,0,40);
		mCamera->lookAt(0,0,-50);
		mCamera->setFarClipDistance(10000);
		mCamera->setNearClipDistance(0.1);
		mViewport = mWindow->addViewport(mCamera);

		
		// setup some basic lighting for our scene
        mSceneMgr->setAmbientLight(ColourValue(0.3, 0.3, 0.3));
        mSceneMgr->createLight()->setPosition(20, 80, 50);
        
		// create a skyplane 5000 units away, facing down, 10000 square units large, with 3x texture tiling
        //mSceneMgr->setSkyPlane(true, Plane(0, -1, 0, 5000), "Examples/SpaceSkyPlane", 10000, 3);

		// Create the particle systems and attach them to a SceneNode
		ParticleUniverse::ParticleSystemManager* pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleSystem* pSys1 = pManager->createParticleSystem("pSys1", "mp_torch", mSceneMgr);
		ParticleUniverse::ParticleSystem* pSys2 = pManager->createParticleSystem("pSys2", "mp_torch", mSceneMgr);
		mSceneMgr->getRootSceneNode()->attachObject(pSys1);
		mSceneMgr->getRootSceneNode()->attachObject(pSys2);

		// Scale the particle systems, just because we can!
		pSys1->setScaleVelocity(10);
		pSys1->setScale(Ogre::Vector3(10, 10, 10));
		pSys2->setScaleVelocity(10);
		pSys2->setScale(Ogre::Vector3(10, 10, 10));

		// Adjust the position of the particle systems a bit by repositioning their ParticleTechnique (there is only one technique in mp_torch)
		// Normally you would do that by setting the position of the SceneNode to which the Particle System is attached, but in this
		// demo they are both attached to the same rootnode.
		pSys1->getTechnique(0)->position = Ogre::Vector3(5, 0, 0);
		pSys2->getTechnique(0)->position = Ogre::Vector3(-5, 0, 0);

		// Start!
		pSys1->start();
		pSys2->start();

		// Look at the torches
		mCamera->setPosition(0, 0, 150);
		mCamera->lookAt(mSceneMgr->getRootSceneNode()->_getDerivedPosition());
	}

	virtual bool frameRenderingQueued (const Ogre::FrameEvent& evt)
	{
		Ogre::Vector3 movementVector (0,0,0);
		if (mOISKeyboard->isKeyDown(OIS::KC_W))
			movementVector.z = -1;
		else if (mOISKeyboard->isKeyDown(OIS::KC_S))
			movementVector.z = 1;
		if (mOISKeyboard->isKeyDown(OIS::KC_A))
			movementVector.x = -1;
		else if (mOISKeyboard->isKeyDown(OIS::KC_D))
			movementVector.x = 1;
		movementVector *= evt.timeSinceLastFrame * 200;

		mCamera->moveRelative(movementVector);

		return BaseApplication::frameRenderingQueued(evt);
	}

	virtual bool mouseMoved(const OIS::MouseEvent& event)
	{
		mCamera->yaw(Ogre::Radian(-event.state.X.rel)*0.01);
		mCamera->pitch(Ogre::Radian(-event.state.Y.rel)*0.01);

		return true;
	}

/** -----------------------------------------------------------------------
	Delete the particle systems
	-----------------------------------------------------------------------*/
	void destroyScene()
	{
		mSceneMgr->getRootSceneNode()->detachAllObjects();
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyAllParticleSystems(mSceneMgr);
	}

/** -----------------------------------------------------------------------
	1. Pause (F9 key) and resume (F10 key)
	-----------------------------------------------------------------------*/
	virtual bool keyPressed(const OIS::KeyEvent& evt)
	{
		if (evt.key == OIS::KC_ESCAPE)
		{
			mShutdown = true;
			return true;
		}
		
		ParticleUniverse::ParticleSystemManager* pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleSystem* pSys1 = pManager->getParticleSystem("pSys1");
		ParticleUniverse::ParticleSystem* pSys2 = pManager->getParticleSystem("pSys2");

		if (evt.key == OIS::KC_PAUSE)
		{
			// Get the particle systems by name and pause them
			if (pSys1)
			{
				pSys1->pause();
			}
			if (pSys2)
			{
				pSys2->pause();
			}
		}
		else if (evt.key == OIS::KC_HOME)
		{
			// Get the particle systems by name and start them again
			if (pSys1)
			{
				pSys1->resume();
			}
			if (pSys2)
			{
				pSys2->resume();
			}
		}
		return true;
	}
};

#endif
