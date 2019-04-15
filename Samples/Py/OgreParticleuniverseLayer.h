#ifndef __ParticleUniverseLayer_H__
#define __ParticleUniverseLayer_H__
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniversePlugin.h"
#include <map>
#include <string>

class OgrePartUniLayer {
public:
	Ogre::SceneManager* mSceneManger;
	ParticleUniverse::ParticleSystemManager* pManager;
	std::map<std::string, ParticleUniverse::ParticleSystem*>particles_map;
	/*
	install plugin of particle universe
	loading the dynamic library
	*/
	void installPlugin(Ogre::Root*);
	/*
	attach SceneManager
	set a SceneManager for this layer to hanle particles
	*/
	void init(Ogre::SceneManager*);
	/*
	destroy the singlePtr and release all the object
	*/
	void destroy();
	/*
	create a new particle;
	*/
	void createNewParticle(std::string, std::string);
	/*
	attach particle to scenenode
	*/
	void attachParticleToSceneNode(std::string, Ogre::SceneNode* );
	void attachEntityBone(std::string, Ogre::Entity*, std::string);

	/*
	set particle scale_velocity
	*/
	void setScaleVelocity(std::string, int);
	/*
	set
	*/
	void setScale(std::string, Ogre::Vector3);
	/*
	set Technique Position
	*/
	void setTechniquePosition(std::string, int , Ogre::Vector3);
	/*
	
	*/
	void start(std::string);
	void pause(std::string);
	void resume(std::string);
	void stop(std::string);
	void stopfade(std::string);
	void stopfade(std::string, Ogre::Real);
};


#endif