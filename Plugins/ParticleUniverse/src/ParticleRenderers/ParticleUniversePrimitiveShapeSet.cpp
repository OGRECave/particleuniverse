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

#include "ParticleRenderers/ParticleUniversePrimitiveShapeSet.h"
#include "OgreMaterialManager.h"
#include "OgreSceneManager.h"
#include "OgreNode.h"
#include "OgreTechnique.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	PrimitiveShapeSet::PrimitiveShapeSet(void) :
		mBoundingRadius(0.0f), 
		mWorldSpace(false),
		mCullIndividual(false),
		mZRotated(false),
		mAllDefaultSize(true)
	{
		setMaterialName("BaseWhite");
		mCastShadows = false;
	}
	//-----------------------------------------------------------------------
	PrimitiveShapeSet::PrimitiveShapeSet(const String& name, unsigned int poolSize, bool externalData) :
		MovableObject(name),
		mBoundingRadius(0.0f), 
		mWorldSpace(false),
		mCullIndividual(false),
		mZRotated(false),
		mAllDefaultSize(true)
	{
		setMaterialName("BaseWhite");
		mCastShadows = false;
	}
	//-----------------------------------------------------------------------
	PrimitiveShapeSet::~PrimitiveShapeSet(void)
	{
	}
	//-----------------------------------------------------------------------
	void PrimitiveShapeSet::setMaterialName(const String& name)
	{
		mMaterialName = name;

		if (ParticleSystemManager::getSingletonPtr()->isAutoLoadMaterials())
		{
			mpMaterial = Ogre::MaterialManager::getSingleton().getByName(name);

			if (!mpMaterial)
				EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Could not find material " + name,
				"PrimitiveShapeSet::setMaterialName" );
	
			/** Load material if not already loaded
			*/
			mpMaterial->load();
		}
	}
	//-----------------------------------------------------------------------
	const String& PrimitiveShapeSet::getMaterialName(void) const
	{
		return mMaterialName;
	}
	//-----------------------------------------------------------------------
	void PrimitiveShapeSet::_notifyCurrentCamera(Camera* cam)
    {
		Ogre::MovableObject::_notifyCurrentCamera(cam);
		mCurrentCamera = cam;
    }
	//-----------------------------------------------------------------------
	const AxisAlignedBox& PrimitiveShapeSet::getBoundingBox(void) const
	{
		return mAABB;
	}
	//-----------------------------------------------------------------------
	const Ogre::MaterialPtr& PrimitiveShapeSet::getMaterial(void) const
	{
		return mpMaterial;
	}
	//-----------------------------------------------------------------------
	void PrimitiveShapeSet::_notifyResized(void)
	{
		mAllDefaultSize = false;
	}
	//-----------------------------------------------------------------------
	void PrimitiveShapeSet::getWorldTransforms(Ogre::Matrix4* xform) const
	{
		if (mWorldSpace)
		{
			*xform = Ogre::Matrix4::IDENTITY;
		}
		else
		{
			*xform = _getParentNodeFullTransform();
		}
	}
	//-----------------------------------------------------------------------
    const Quaternion& PrimitiveShapeSet::getWorldOrientation(void) const
	{
		return mParentNode->_getDerivedOrientation();
	}
	//-----------------------------------------------------------------------
	const Vector3& PrimitiveShapeSet::getWorldPosition(void) const
	{
		return mParentNode->_getDerivedPosition();
	}
	//-----------------------------------------------------------------------
	void PrimitiveShapeSet::setZRotated(bool zRotated)
	{
		mZRotated = zRotated;
	}
	//-----------------------------------------------------------------------
	bool PrimitiveShapeSet::isZRotated(void) const
	{
		return mZRotated;
	}
	//-----------------------------------------------------------------------
	void PrimitiveShapeSet::_notifyZRotated(void)
	{
		mZRotated = true;
	}
	//-----------------------------------------------------------------------
	void PrimitiveShapeSet::rotateTexture(Real speed)
	{
		// Get the material and rotate it, assume the material is loaded already, otherwise skip.
		Ogre::MaterialPtr material = getMaterial();
		if (!material)
			return;

		Ogre::TextureUnitState::EffectMap::const_iterator it;
		unsigned short numberOfTechniques = material->getNumTechniques();
		for (unsigned short u = 0; u < numberOfTechniques; ++u)
		{
			Ogre::Technique* technique = material->getTechnique(u);
			unsigned short numberOfPasses = technique->getNumPasses();
			for (unsigned short v = 0; v < numberOfPasses; ++v)
			{
				Ogre::Pass* pass = technique->getPass(v);
				unsigned short numberOfTextureUnitStates = pass->getNumTextureUnitStates();
				for (unsigned short w = 0; w < numberOfTextureUnitStates; ++w)
				{
					// Set the rotation if not already available.
					// This can only be done once! Changing the rotationspeed or removing the rotation
					// and resetting it doesn�t seem to work.
					Ogre::TextureUnitState* textureUnitState = pass->getTextureUnitState(w);
					it = textureUnitState->getEffects().find(Ogre::TextureUnitState::ET_ROTATE);
					if (it == textureUnitState->getEffects().end())
					{
						textureUnitState->setRotateAnimation(speed);
					}
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	bool PrimitiveShapeSet::isCullIndividually(void) const
	{
		return mCullIndividual;
	}
	//-----------------------------------------------------------------------
	void PrimitiveShapeSet::setCullIndividually(bool cullIndividual)
	{
		mCullIndividual = cullIndividual;
	}
	//-----------------------------------------------------------------------
	uint32 PrimitiveShapeSet::getTypeFlags(void) const
	{
		return Ogre::SceneManager::FX_TYPE_MASK;
	}
	//-----------------------------------------------------------------------
	Real PrimitiveShapeSet::getSquaredViewDepth(const Camera* const cam) const
	{
		assert(mParentNode);
		return mParentNode->getSquaredViewDepth(cam);
	}
	//-----------------------------------------------------------------------
	Real PrimitiveShapeSet::getBoundingRadius(void) const
	{
		return mBoundingRadius;
	}
	//-----------------------------------------------------------------------
	const Ogre::LightList& PrimitiveShapeSet::getLights(void) const
	{
		return queryLights();
	}

}
