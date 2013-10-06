#ifndef __ParticleUniverseDemo_H__
#define __ParticleUniverseDemo_H__

#include "SdkSample.h"
#include "ParticleUniverseSystemManager.h"

using namespace Ogre;
using namespace OgreBites;

/** -----------------------------------------------------------------------------------------------------------------
	This class represents a demo for the SampleBrowser. Perform the following actions to run this demo:

	1. 
	Add the resources needed for this demo to bin/release/resources.cfg and to bin/debug/resources.cfg. Add the following 
	settings (asume that the Particle Universe Editor is installed):
        FileSystem=../../ParticleUniverseEditor/media/examples/scripts
        FileSystem=../../ParticleUniverseEditor/media/examples/materials
        FileSystem=../../ParticleUniverseEditor/media/examples/models
        FileSystem=../../ParticleUniverseEditor/media/examples/textures
	
	2.
	Add the entry
	    Plugin=ParticleUniverse
	to bin/release/plugins.cfg
	
	3.
	Add the entry
	    Plugin=ParticleUniverse_d
	to bin/debug/plugins.cfg

	4.
	Add the entry
	    SamplePlugin=Sample_ParticleUniverseDemo
	to bin/release/samples.cfg

	5.
	Add the entry
	    SamplePlugin=Sample_ParticleUniverseDemo_d
	to bin/debug/samples.cfg

	6.
	Compile the Sample_ParticleUniverseDemo (in debug and/or release). Make sure you compiled the ParticleUniverse project first.

	7.
	Run the SampleBrowser_d.exe and/or SampleBrowser.exe.
	-----------------------------------------------------------------------------------------------------------------*/
class _OgreSampleClassExport Sample_ParticleUniverseDemo : public SdkSample
{
public:

	Sample_ParticleUniverseDemo()
	{
		mInfo["Title"] = "Particle Universe Demo";
		mInfo["Description"] = "Demo to create, start and destroy a Particle Universe particle system.";
		mInfo["Thumbnail"] = "thumb_particles.png";
		mInfo["Category"] = "Particle Universe";
	}

protected:

/** -----------------------------------------------------------------------
	Create some particle systems
	-----------------------------------------------------------------------*/
	void setupContent()
	{     
		// setup some basic lighting for our scene
        mSceneMgr->setAmbientLight(ColourValue(0.3, 0.3, 0.3));
        mSceneMgr->createLight()->setPosition(20, 80, 50);
        
		// create a skyplane 5000 units away, facing down, 10000 square units large, with 3x texture tiling
        mSceneMgr->setSkyPlane(true, Plane(0, -1, 0, 5000), "Examples/SpaceSkyPlane", 10000, 3);

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

/** -----------------------------------------------------------------------
	Delete the particle systems
	-----------------------------------------------------------------------*/
	void cleanupContent()
	{
		mSceneMgr->getRootSceneNode()->detachAllObjects();
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyAllParticleSystems(mSceneMgr);
	}

/** -----------------------------------------------------------------------
	1. Pause (F9 key) and resume (F10 key)
	-----------------------------------------------------------------------*/
	virtual bool keyPressed(const OIS::KeyEvent& evt)
	{
		SdkSample::keyPressed(evt);
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
