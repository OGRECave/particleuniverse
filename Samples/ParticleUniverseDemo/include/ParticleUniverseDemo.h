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
#include <OgreTrays.h>

#include "ParticleUniverseRoot.h"

using namespace Ogre;

class Sample_ParticleUniverseDemo : public OgreBites::ApplicationContext, public OgreBites::InputListener, public OgreBites::TrayListener
{
public:
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	OgreBites::CameraMan* mCameraMan;

	OgreBites::TrayManager* mTrayMgr;

	OgreBites::InputListenerChain mInputListners;

	// ApplicationContext Boilerplate
	Sample_ParticleUniverseDemo() : OgreBites::ApplicationContext("PU Demo") {
	}

	void createRoot() {
	    OgreBites::ApplicationContext::createRoot();
	    new ParticleUniverse::ParticleUniverseRoot();
		ParticleUniverse::ParticleUniverseRoot::getSingleton().initialise();
	}

	/** -----------------------------------------------------------------------
	Create some particle systems
	-----------------------------------------------------------------------*/
	void createScene()
	{
	    addInputListener(this);
		mSceneMgr = getRoot()->createSceneManager("DefaultSceneManager");

		LogManager::getSingleton().getDefaultLog()->logMessage("Registering SceneMgr with RTSS");
		
		// register our mSceneMgr with the RTSS (GL3+ requires these lines)
		Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
		shadergen->addSceneManager(mSceneMgr);

		mCamera = mSceneMgr->createCamera("Camera");
		SceneNode* camNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		camNode->attachObject(mCamera);
		mCameraMan = new OgreBites::CameraMan(camNode);
		mCameraMan->setStyle(OgreBites::CS_ORBIT);

		mCamera->setFarClipDistance(10000);
		mCamera->setNearClipDistance(5);
		Ogre::Viewport* viewport = getRenderWindow()->addViewport(mCamera);
		viewport->setBackgroundColour(ColourValue(0.3, 0.3, 0.3));

		mSceneMgr->addRenderQueueListener(getOverlaySystem());
		
		// setup some basic lighting for our scene
        mSceneMgr->setAmbientLight(ColourValue(0.1, 0.1, 0.1));
        mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(20, 80, 50))->attachObject(mSceneMgr->createLight());
        
		// create a skyplane 5000 units away, facing down, 10000 square units large, with 3x texture tiling
        //mSceneMgr->setSkyPlane(true, Plane(0, -1, 0, 5000), "Examples/SpaceSkyPlane", 10000, 3);

		// Create the particle systems and attach them to a SceneNode
		ParticleUniverse::ParticleSystemManager* pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleSystem* pSys = pManager->createParticleSystem("pSys1", "mp_torch", mSceneMgr);

		mTrayMgr = new OgreBites::TrayManager("Interface", getRenderWindow(), this);

		mInputListners = OgreBites::InputListenerChain({mTrayMgr, mCameraMan});
		addInputListener(&mInputListners);

		std::vector<String> templateNames;
		pManager->particleSystemTemplateNames(templateNames);
        auto menu = mTrayMgr->createLongSelectMenu(OgreBites::TL_TOPLEFT, "MaterialSelectMenu", "Particle System", 350, 200, 15, templateNames);
		menu->selectItem("mp_torch", false);

		// Adjust the position of the particle systems a bit
		mSceneMgr->getRootSceneNode()->attachObject(pSys);

		// Scale the particle systems, just because we can!
		pSys->setScaleVelocity(10);
		pSys->setScale(Ogre::Vector3(10, 10, 10));
		// Start!
		pSys->start();

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

	void itemSelected(OgreBites::SelectMenu* menu)
	{
		auto pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		auto templateName = menu->getSelectedItem();

		pManager->destroyParticleSystem("pSys1", mSceneMgr);
		auto pSys = pManager->createParticleSystem("pSys1", templateName, mSceneMgr);
		mSceneMgr->getRootSceneNode()->attachObject(pSys);
		pSys->setScaleVelocity(10);
		pSys->setScale(Vector3(10, 10, 10));
		pSys->start();
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

		if (evt.keysym.sym == SDLK_F9)
		{
			// Get the particle systems by name and pause them
			if (pSys1)
			{
				pSys1->pause();
			}
		}
		else if (evt.keysym.sym == SDLK_F10)
		{
			// Get the particle systems by name and start them again
			if (pSys1)
			{
				pSys1->resume();
			}
		}
		return true;
	}
};

#endif
