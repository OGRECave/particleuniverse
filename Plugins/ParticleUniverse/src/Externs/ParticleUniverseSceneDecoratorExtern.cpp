/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2013 Henry van Merode. All rights reserved.

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

#include "ParticleUniversePCH.h"

#ifndef PARTICLE_UNIVERSE_EXPORTS
#define PARTICLE_UNIVERSE_EXPORTS
#endif

#include "Externs/ParticleUniverseSceneDecoratorExtern.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	SceneDecoratorExtern::SceneDecoratorExtern(void) :  
		Extern(),
		mEntity(0),
		mEntitySet(false),
		mMeshName(BLANK_STRING),
		mEntityName(BLANK_STRING),
		mMaterialName("BaseWhite"),
		mMaterialNameSet(false),
		mScale(Vector3::UNIT_SCALE),
		mSubnode(0),
		mPosition(Vector3::ZERO),
		mCount(0)
	{
	}
	//-----------------------------------------------------------------------
	void SceneDecoratorExtern::_prepare(ParticleTechnique* technique)
	{
		if (mEntitySet)
		{
			if (!mParentTechnique->getParentSystem())
			{
				return;
			}
			
			// Attach entity to a child scenenode. If the parent system is attached to a TagPoint, the entity cannot be attached.
			Ogre::SceneNode* sceneNode = mParentTechnique->getParentSystem()->getParentSceneNode();
			if (sceneNode)
			{
				std::stringstream ss; 
				ss << this;
				String sceneNodeName = "ParticleUniverse" + ss.str() + StringConverter::toString(mCount++);
				mSubnode = sceneNode->createChildSceneNode(sceneNodeName);
				mSubnode->setScale(mScale);
				mSubnode->setPosition(mPosition);
				if (!mEntity)
				{
					createEntity();
					mSubnode->attachObject(mEntity);
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	void SceneDecoratorExtern::_unprepare(ParticleTechnique* technique)
	{
		// Todo
	}
	//-----------------------------------------------------------------------
	void SceneDecoratorExtern::_interface(ParticleTechnique* technique, 
		Particle* particle, 
		Real timeElapsed)
	{
		// Nothing to do yet
	}
	//-----------------------------------------------------------------------
	void SceneDecoratorExtern::_notifyStart (void)
	{
		// Scale the node
		if (!mSubnode && mParentTechnique->getParentSystem())
		{
			std::stringstream ss; 
			ss << this;
			String sceneNodeName = "ParticleUniverse" + ss.str() + StringConverter::toString(mCount++);
			Ogre::SceneNode* sceneNode = mParentTechnique->getParentSystem()->getParentSceneNode();
			mSubnode = sceneNode->createChildSceneNode(sceneNodeName);
		}

		if (!mEntity)
		{
			createEntity();
			mSubnode->attachObject(mEntity);
		}

		mSubnode->setScale(mScale);
		mSubnode->setPosition(mPosition);
	}
	//-----------------------------------------------------------------------
	void SceneDecoratorExtern::_notifyStop (void)
	{
		if (mSubnode && mParentTechnique->getParentSystem())
		{
			Ogre::SceneNode* sceneNode = mParentTechnique->getParentSystem()->getParentSceneNode();
			Ogre::SceneNode::ChildNodeIterator it = sceneNode->getChildIterator();
			unsigned int i = 0;
			while (it.hasMoreElements()) 
			{ 
				Ogre::SceneNode* child = static_cast<Ogre::SceneNode*>(it.getNext());
				if (child == mSubnode)
				{
					mSubnode->detachAllObjects();
					sceneNode->removeAndDestroyChild(i);
					mSubnode = 0;
				}
				++i;
			}

			destroyEntity();
		}
	}
	//-----------------------------------------------------------------------
	void SceneDecoratorExtern::createEntity(void)
	{
		if (!mEntity)
		{
			Ogre::SceneManager* sceneManager = mParentTechnique->getParentSystem()->getSceneManager();
			if (sceneManager)
			{
				std::stringstream ss;
				ss << this;
				mEntityName = mMeshName + ss.str();
				mEntity = sceneManager->createEntity(mEntityName, mMeshName);
				if (mMaterialNameSet)
				{
					mEntity->setMaterialName(mMaterialName);
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	void SceneDecoratorExtern::destroyEntity(void)
	{
		if (mEntity)
		{
			Ogre::SceneManager* sceneManager = mParentTechnique->getParentSystem()->getSceneManager();
			if (sceneManager)
			{
				sceneManager->destroyEntity(mEntityName);
				mEntity = 0;
				mEntityName = BLANK_STRING;
			}
		}
	}
	//-----------------------------------------------------------------------
	const String& SceneDecoratorExtern::getMeshName(void) const
	{
		return mMeshName;
	}
	//-----------------------------------------------------------------------
	void SceneDecoratorExtern::setMeshName(const String& meshName)
	{
		mMeshName = meshName;
		mEntitySet = true;
	}
	//-----------------------------------------------------------------------
	const String& SceneDecoratorExtern::getMaterialName(void) const
	{
		return mMaterialName;
	}
	//-----------------------------------------------------------------------
	void SceneDecoratorExtern::setMaterialName(const String& materialName)
	{
		mMaterialName = materialName;
		mMaterialNameSet = true;
	}
	//-----------------------------------------------------------------------
	const Vector3& SceneDecoratorExtern::getScale(void) const
	{
		return mScale;
	}
	//-----------------------------------------------------------------------
	void SceneDecoratorExtern::setScale(Vector3& scale)
	{
		mScale = scale;
	}
	//-----------------------------------------------------------------------
	const Vector3& SceneDecoratorExtern::getPosition(void) const
	{
		return mPosition;
	}
	//-----------------------------------------------------------------------
	void SceneDecoratorExtern::setPosition(Vector3& position)
	{
		mPosition = position;
	}
	//-----------------------------------------------------------------------
	void SceneDecoratorExtern::copyAttributesTo (Extern* externObject)
	{
		Extern::copyAttributesTo(externObject);
		SceneDecoratorExtern* sceneDecoratorExtern = static_cast<SceneDecoratorExtern*>(externObject);
		sceneDecoratorExtern->setMaterialName(mMaterialName);
		sceneDecoratorExtern->setMeshName(mMeshName);
		sceneDecoratorExtern->mScale = mScale;
		sceneDecoratorExtern->mPosition = mPosition;
	}
}
