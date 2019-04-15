#include "OgreParticleuniverseLayer.h"
#include "Ogre.h"

void OgrePartUniLayer::installPlugin(Ogre::Root * root) {
	root->installPlugin(new ParticleUniverse::ParticleUniversePlugin);
}

void OgrePartUniLayer::init(Ogre::SceneManager* mgr) {
	mSceneManger = mgr;
	pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
}

void OgrePartUniLayer::destroy() {
	mSceneManger->getRootSceneNode()->detachAllObjects();
	ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyAllParticleSystems(mSceneManger);
}

void OgrePartUniLayer::createNewParticle(std::string name, std::string particle_name) {
	particles_map.insert(std::pair<std::string, ParticleUniverse::ParticleSystem*>(name, pManager->createParticleSystem(name, particle_name, mSceneManger)));
}

void OgrePartUniLayer::attachParticleToSceneNode(std::string name, Ogre::SceneNode* nd) {
	nd->attachObject(particles_map.at(name));
}

void OgrePartUniLayer::attachEntityBone(std::string name, Ogre::Entity *ent, std::string bonename) {
	ent->attachObjectToBone(Ogre::String(bonename), particles_map.at(name));
}

void OgrePartUniLayer::setScaleVelocity(std::string name, int i) {
	particles_map.at(name)->setScaleVelocity(i);
}

void OgrePartUniLayer::setScale(std::string name, Ogre::Vector3 v) {
	particles_map.at(name)->setScale(v);
}

void OgrePartUniLayer::setTechniquePosition(std::string name, int i, Ogre::Vector3 v) {
	particles_map.at(name)->getTechnique(i)->position = v;
}

void OgrePartUniLayer::start(std::string name) {
	particles_map.at(name)->start();
}

void OgrePartUniLayer::pause(std::string name) {
	particles_map.at(name)->pause();
}
void OgrePartUniLayer::resume(std::string name) {
	particles_map.at(name)->resume();
}
void OgrePartUniLayer::stopfade(std::string name) {
	particles_map.at(name)->stopFade();
}
void OgrePartUniLayer::stop(std::string name) {
	particles_map.at(name)->stop();
}
void OgrePartUniLayer::stopfade(std::string name,Ogre::Real t) {
	particles_map.at(name)->stopFade(t);
}
