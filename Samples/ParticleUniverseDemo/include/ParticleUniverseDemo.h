#ifndef __ParticleUniverseDemo_H__
#define __ParticleUniverseDemo_H__

#include "ParticleUniverseSystemManager.h"

#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneNode.h>
#include <OgreApplicationContext.h>
#include <OgreCameraMan.h>

#include "ParticleUniversePlugin.h"

using namespace Ogre;


/*
try to use User class
*/

//#include "OgreParticleuniverseLayer.h"
//
//class Sample_ParticleUniverseDemo : public OgreBites::ApplicationContext, public OgreBites::InputListener
//{
//public:
//	Ogre::SceneManager* mSceneMgr;
//	Ogre::Camera* mCamera;
//	OgreBites::CameraMan* mCameraMan;
//	OgrePartUniLayer layer;
//
//	// ApplicationContext Boilerplate
//	Sample_ParticleUniverseDemo() : OgreBites::ApplicationContext("PU Demo") {
//	}
//
//	void createRoot() {
//	    OgreBites::ApplicationContext::createRoot();
//		layer = OgrePartUniLayer();
//		layer.installPlugin(getRoot());
//	}
//
//	void setupInput(bool) {}
//
//	/** -----------------------------------------------------------------------
//	Create some particle systems
//	-----------------------------------------------------------------------*/
//	void createScene()
//	{
//	    addInputListener(this);
//		mSceneMgr = getRoot()->createSceneManager("DefaultSceneManager");
//		mCamera = mSceneMgr->createCamera("Camera");
//		SceneNode* camNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
//		camNode->attachObject(mCamera);
//		mCameraMan = new OgreBites::CameraMan(camNode);
//		mCameraMan->setStyle(OgreBites::CS_ORBIT);
//		addInputListener(mCameraMan);
//
//		mCamera->setFarClipDistance(10000);
//		mCamera->setNearClipDistance(0.1);
//		Ogre::Viewport* viewport = getRenderWindow()->addViewport(mCamera);
//
//		// setup some basic lighting for our scene
//        mSceneMgr->setAmbientLight(ColourValue(0.3, 0.3, 0.3));
//        //mSceneMgr->createLight()->setPosition(20, 80, 50);
//        
//		// create a skyplane 5000 units away, facing down, 10000 square units large, with 3x texture tiling
//        //mSceneMgr->setSkyPlane(true, Plane(0, -1, 0, 5000), "Examples/SpaceSkyPlane", 10000, 3);
//
//		// Create the particle systems and attach them to a SceneNode
//		layer.init(mSceneMgr);
//		layer.createNewParticle("pSys1", "asdf");
//		layer.createNewParticle("pSys2", "asdf");
//		layer.attachParticleToSceneNode("pSys1", mSceneMgr->getRootSceneNode());
//		layer.attachParticleToSceneNode("pSys2", mSceneMgr->getRootSceneNode());
//
//
//		// Scale the particle systems, just because we can!
//		layer.setScaleVelocity("pSys1", 10);
//		layer.setScale("pSys1", Ogre::Vector3(10, 10, 10));
//		layer.setScaleVelocity("pSys2", 10);
//		layer.setScale("pSys2", Ogre::Vector3(10, 10, 10));
//
//
//		// Adjust the position of the particle systems a bit by repositioning their ParticleTechnique (there is only one technique in mp_torch)
//		// Normally you would do that by setting the position of the SceneNode to which the Particle System is attached, but in this
//		// demo they are both attached to the same rootnode.
//		layer.setTechniquePosition("pSys1", 0, Ogre::Vector3(5, 0, 0));
//		layer.setTechniquePosition("pSys2", 0, Ogre::Vector3(-5, 0, 0));
//
//
//		// Start!
//		layer.start("pSys1");
//		layer.start("pSys2");
//
//		// Look at the torches
//		mCameraMan->setYawPitchDist(Radian(0), Radian(0), 150);
//	}
//
///** -----------------------------------------------------------------------
//	Delete the particle systems
//	-----------------------------------------------------------------------*/
//	void destroyScene()
//	{
//		layer.destroy();
//	}
//
//
//	bool keyPressed(const OgreBites::KeyboardEvent& evt)
//	{
//	    using namespace OgreBites;
//		if (evt.keysym.sym == SDLK_ESCAPE)
//		{
//			getRoot()->queueEndRendering();
//			return true;
//		}
//		
//		ParticleUniverse::ParticleSystemManager* pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
//		ParticleUniverse::ParticleSystem* pSys1 = pManager->getParticleSystem("pSys1");
//		ParticleUniverse::ParticleSystem* pSys2 = pManager->getParticleSystem("pSys2");
//
//		if (evt.keysym.sym == SDLK_F9)
//		{
//			// Get the particle systems by name and pause them
//			if (pSys1)
//			{
//				pSys1->pause();
//			}
//			if (pSys2)
//			{
//				pSys2->pause();
//			}
//		}
//		else if (evt.keysym.sym == SDLK_F10)
//		{
//			// Get the particle systems by name and start them again
//			if (pSys1)
//			{
//				pSys1->resume();
//			}
//			if (pSys2)
//			{
//				pSys2->resume();
//			}
//		}
//		return true;
//	}
//};






class Sample_ParticleUniverseDemo : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	OgreBites::CameraMan* mCameraMan;

	// ApplicationContext Boilerplate
	Sample_ParticleUniverseDemo() : OgreBites::ApplicationContext("PU Demo") {
	}

	void createRoot() {
		OgreBites::ApplicationContext::createRoot();
		getRoot()->installPlugin(new ParticleUniverse::ParticleUniversePlugin());
	}

	void setupInput(bool) {}

	/** -----------------------------------------------------------------------
	Create some particle systems
	-----------------------------------------------------------------------*/
	void createScene()
	{
		addInputListener(this);
		mSceneMgr = getRoot()->createSceneManager("DefaultSceneManager");
		mCamera = mSceneMgr->createCamera("Camera");
		SceneNode* camNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		camNode->attachObject(mCamera);
		mCameraMan = new OgreBites::CameraMan(camNode);
		mCameraMan->setStyle(OgreBites::CS_ORBIT);
		addInputListener(mCameraMan);

		mCamera->setFarClipDistance(10000);
		mCamera->setNearClipDistance(0.1);
		Ogre::Viewport* viewport = getRenderWindow()->addViewport(mCamera);

		// setup some basic lighting for our scene
		mSceneMgr->setAmbientLight(ColourValue(0.3, 0.3, 0.3));
		//mSceneMgr->createLight()->setPosition(20, 80, 50);

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
		mCameraMan->setYawPitchDist(Radian(0), Radian(0), 150);
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
	bool keyPressed(const OgreBites::KeyboardEvent& evt)
	{
		using namespace OgreBites;
		if (evt.keysym.sym == SDLK_ESCAPE)
		{
			getRoot()->queueEndRendering();
			return true;
		}

		ParticleUniverse::ParticleSystemManager* pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleSystem* pSys1 = pManager->getParticleSystem("pSys1");
		ParticleUniverse::ParticleSystem* pSys2 = pManager->getParticleSystem("pSys2");

		if (evt.keysym.sym == SDLK_F9)
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
		else if (evt.keysym.sym == SDLK_F10)
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
