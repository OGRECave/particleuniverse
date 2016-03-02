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

#include "ParticleRenderers/ParticleUniverseRibbonTrailRenderer.h"
#include "OgreSceneManager.h"

namespace ParticleUniverse
{
	// Constants
	const bool RibbonTrailRenderer::DEFAULT_USE_VERTEX_COLOURS = true;
	const size_t RibbonTrailRenderer::DEFAULT_MAX_ELEMENTS = 10;
	const Real RibbonTrailRenderer::DEFAULT_LENGTH = 400;
	const Real RibbonTrailRenderer::DEFAULT_WIDTH = 5;
	const bool RibbonTrailRenderer::DEFAULT_RANDOM_INITIAL_COLOUR = true;
	const ColourValue RibbonTrailRenderer::DEFAULT_INITIAL_COLOUR(1, 1, 1);
	const ColourValue RibbonTrailRenderer::DEFAULT_COLOUR_CHANGE = ColourValue(0.5, 0.5, 0.5, 0.5);

	//-----------------------------------------------------------------------
	RibbonTrailRenderer::RibbonTrailRenderer(void) : 
		ParticleRenderer(),
		mQuota(0),
		mTrail(0),
		mUseVertexColours(DEFAULT_USE_VERTEX_COLOURS),
		mMaxChainElements(DEFAULT_MAX_ELEMENTS),
		mSetLength(false),
		mTrailLength(DEFAULT_LENGTH),
		mSetWidth(false),
		mTrailWidth(DEFAULT_WIDTH),
		mRandomInitialColour(DEFAULT_RANDOM_INITIAL_COLOUR),
		mInitialColour(DEFAULT_INITIAL_COLOUR),
		mColourChange(DEFAULT_COLOUR_CHANGE),
		mChildNode(0)
	{
		std::stringstream ss; 
		ss << this;
		mRibbonTrailName = "RibbonTrail" + ss.str();
		autoRotate = false;
	}
	//-----------------------------------------------------------------------
	RibbonTrailRenderer::~RibbonTrailRenderer(void)
	{
		if (!mParentTechnique)
			return;
		
		_destroyAll();
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::_notifyRescaled(const Vector3& scale)
	{
		ParticleRenderer::_notifyRescaled(scale);
		if (mTrail)
		{
			// Use the average length (is there a good alternative?
			mTrail->setTrailLength(scale.y * mTrailLength);
			size_t numberOfChains = mTrail->getNumberOfChains();
			for (size_t i = 0; i < numberOfChains; ++i)
			{
				mTrail->setInitialWidth(i, scale.x * mTrailWidth);
			}
		}
	}
	//-----------------------------------------------------------------------
	bool RibbonTrailRenderer::isUseVertexColours(void) const
	{
		return mUseVertexColours;
	} 
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::setUseVertexColours(bool useVertexColours)
	{
		mUseVertexColours = useVertexColours;
	} 
	//-----------------------------------------------------------------------
	size_t RibbonTrailRenderer::getMaxChainElements(void) const
	{
		return mMaxChainElements;
	} 
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::setMaxChainElements(size_t maxChainElements)
	{
		mMaxChainElements = maxChainElements;
	} 
	//-----------------------------------------------------------------------
	Real RibbonTrailRenderer::getTrailLength(void) const
	{
		return mTrailLength;
	} 
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::setTrailLength(Real trailLength)
	{
		mTrailLength = trailLength;
		mSetLength = true;
	} 
	//-----------------------------------------------------------------------
	Real RibbonTrailRenderer::getTrailWidth(void) const
	{
		return mTrailWidth;
	} 
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::setTrailWidth(Real trailWidth)
	{
		mTrailWidth = trailWidth;
		mSetWidth = true;
	} 
	//-----------------------------------------------------------------------
	bool RibbonTrailRenderer::isRandomInitialColour(void) const
	{
		return mRandomInitialColour;
	} 
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::setRandomInitialColour(bool randomInitialColour)
	{
		mRandomInitialColour = randomInitialColour;
	} 
	//-----------------------------------------------------------------------
	const ColourValue& RibbonTrailRenderer::getInitialColour(void) const
	{
		return mInitialColour;
	} 
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::setInitialColour(const ColourValue& initialColour)
	{
		mInitialColour = initialColour;
	} 
	//-----------------------------------------------------------------------
	const ColourValue& RibbonTrailRenderer::getColourChange(void) const
	{
		return mColourChange;
	} 
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::setColourChange(const ColourValue& colourChange)
	{
		mColourChange = colourChange;
	} 
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::_destroyAll(void)
	{
		if (!mParentTechnique)
			return;

		// Remove the listener
		mParentTechnique->removeTechniqueListener(this);
		
		// Detach the Ribbontrail
		if (mChildNode && mTrail && mTrail->isAttached())
		{
			mChildNode->detachObject(mTrail);
		}

		// Delete the Ribbontrail
		Ogre::SceneManager* sceneManager = mParentTechnique->getParentSystem()->getSceneManager();
		if (mTrail && sceneManager && sceneManager->hasRibbonTrail(mRibbonTrailName))
		{
			sceneManager->destroyRibbonTrail(mRibbonTrailName);
			mTrail = 0;
		}

		// Delete the visual data
		vector<RibbonTrailRendererVisualData*>::const_iterator it;
		vector<RibbonTrailRendererVisualData*>::const_iterator itEnd = mAllVisualData.end();
		for (it = mAllVisualData.begin(); it != itEnd; ++it)
		{
			PU_DELETE_T(*it, RibbonTrailRendererVisualData, MEMCATEGORY_SCENE_OBJECTS);
		}

		mAllVisualData.clear();
		mVisualData.clear();

		// Reset the visual data in the pool
		mParentTechnique->initVisualDataInPool();

		// Destroy the children of the childnode, but not the childnode itself, because this gives a delete-order problem
		if (mChildNode)
		{
			mChildNode->removeAndDestroyAllChildren();
		}
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::setVisible(bool visible)
	{
		if (mTrail)
		{
			mTrail->setVisible(visible);
		}
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::_prepare(ParticleTechnique* technique)
	{
		if (!technique || mRendererInitialised)
			return;

		// Register itself to the technique
		if (mParentTechnique)
		{
			// Although it is safe to assume that technique == mParentTechnique, use the mParentTechnique, because the mParentTechnique is
			// also used for unregistering.
			mParentTechnique->addTechniqueListener(this);
		}

		mQuota = technique->getVisualParticleQuota();
		if (!mChildNode)
		{
			// Create a childnode if not created earlier
			std::stringstream ss; 
			ss << this;
			String childNodeNodeName = "ParticleUniverse" + ss.str();
			mChildNode = mParentTechnique->getParentSystem()->getParentSceneNode()->createChildSceneNode(childNodeNodeName);
			mChildNode->setInheritOrientation(false);
		}

		if (mChildNode)
		{
			// Create RibbonTrail
			Ogre::SceneManager* sceneManager = mParentTechnique->getParentSystem()->getSceneManager();
			mTrail = sceneManager->createRibbonTrail(mRibbonTrailName);
			mTrail->setNumberOfChains(mQuota);
			mTrail->setMaxChainElements(mMaxChainElements);
			mTrail->setMaterialName(technique->getMaterialName());
			mTrail->setRenderQueueGroup(mQueueId);
			if (mSetLength)
			{
				mTrail->setTrailLength(_mRendererScale.y * mTrailLength);
			}
			else
			{
				mTrail->setTrailLength(_mRendererScale.y * technique->getDefaultHeight());
			}
			mTrail->setUseVertexColours(mUseVertexColours);

			// By default, make this transparent to query requests
			mTrail->setQueryFlags(0);

			// Create number of VisualData objects
			String sceneNodeName;
			std::stringstream ss; 
			ss << this;
			for (size_t i = 0; i < mQuota; i++)
			{
				sceneNodeName = "ParticleUniverse" + ss.str() + StringConverter::toString(i);
				RibbonTrailRendererVisualData* visualData = 
					PU_NEW_T(RibbonTrailRendererVisualData, MEMCATEGORY_SCENE_OBJECTS)(mChildNode->createChildSceneNode(sceneNodeName), mTrail);
				visualData->node->setInheritOrientation(false);
				visualData->index = i;
				mAllVisualData.push_back(visualData); // Managed by this renderer
				mVisualData.push_back(visualData); // Used to assign to a particle
				if (mRandomInitialColour)
				{
					mTrail->setInitialColour(i, Math::UnitRandom(), Math::UnitRandom(), Math::UnitRandom());
				}
				else
				{
					mTrail->setInitialColour(i, mInitialColour);
				}
				mTrail->setColourChange(i, mColourChange);
				if (mSetWidth)
				{
					mTrail->setInitialWidth(i, _mRendererScale.x * mTrailWidth);
				}
				else
				{
					mTrail->setInitialWidth(i, _mRendererScale.x * mParentTechnique->getDefaultWidth());
				}
			}
			mChildNode->attachObject(mTrail);
			mRendererInitialised = true;
		}
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::_unprepare(ParticleTechnique* technique)
	{
		// Todo: The childnodes still exist, but because the parent node can also have other childnodes, they need to be deleted with caution
		mRendererInitialised = false; // Retriggers the _prepare function
		_destroyAll(); // Delete all nodes, they will be rebuild
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::_updateRenderQueue(Ogre::RenderQueue* queue, ParticlePool* pool)
	{
		// Always perform this one
		ParticleRenderer::_updateRenderQueue(queue, pool);

		if (!mVisible)
			return;

		Particle* particle = static_cast<Particle*>(pool->getFirst());
		RibbonTrailRendererVisualData* visualData = 0;
		while (!pool->end())
		{
			if (particle)
			{
				if (!particle->visualData && !mVisualData.empty())
				{
					particle->visualData = mVisualData.back();
					mVisualData.pop_back();
					visualData = static_cast<RibbonTrailRendererVisualData*>(particle->visualData);
				}
				visualData = static_cast<RibbonTrailRendererVisualData*>(particle->visualData);
				if (visualData)
				{
					Ogre::SceneNode* node = visualData->node;
					node->_setDerivedPosition(particle->position);

					// Set the width of the trail if required
					if (particle->particleType == Particle::PT_VISUAL)
					{
						VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
						if (visualParticle->ownDimensions)
						{
							mTrail->setInitialWidth(visualData->index, visualParticle->width);
						}
					}
					visualData->setVisible(true);
				}
			}
			particle = static_cast<Particle*>(pool->getNext());
		}
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::_notifyAttached(Ogre::Node* parent, bool isTagPoint)
	{
		// No implementation here
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::_setMaterialName(const String& materialName)
	{
		if (mTrail)
		{
			mTrail->setMaterialName(materialName);
		}
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::_notifyCurrentCamera(Camera* cam)
	{
		if (mTrail)
		{
			mTrail->_notifyCurrentCamera(cam);
		}
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::_notifyParticleQuota(size_t quota)
	{
		mRendererInitialised = false; // Retriggers the _prepare function
		_destroyAll(); // Delete all nodes, they will be rebuild
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::_notifyDefaultDimensions(Real width, Real height, Real depth)
	{
		// No implementation
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::_notifyParticleResized(void)
	{
		// No implementation
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::_notifyParticleZRotated(void)
	{
		// No implementation
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::setRenderQueueGroup(uint8 queueId)
	{
		mQueueId = queueId;
		if (mTrail)
		{
			mTrail->setRenderQueueGroup(queueId);
		}
	}
	//-----------------------------------------------------------------------
	SortMode RibbonTrailRenderer::_getSortMode(void) const
	{
		return SM_DISTANCE; // In fact there is no sorting
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::particleEmitted(ParticleTechnique* particleTechnique, Particle* particle)
	{
		// Assigning visual data already done in _updateRenderQueue()
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::particleExpired(ParticleTechnique* particleTechnique, Particle* particle)
	{
		if (particle->visualData)
		{
			RibbonTrailRendererVisualData* ribbonTrailRendererVisualData = static_cast<RibbonTrailRendererVisualData*>(particle->visualData);
			ribbonTrailRendererVisualData->setVisible(false);
		}
	}
	//-----------------------------------------------------------------------
	void RibbonTrailRenderer::copyAttributesTo (ParticleRenderer* renderer)
	{
		// First copy parent attributes
		ParticleRenderer::copyAttributesTo(renderer);

		// First cast to RibbonTrailRenderer
		RibbonTrailRenderer* ribbonTrailRenderer = static_cast<RibbonTrailRenderer*>(renderer);
		ribbonTrailRenderer->mUseVertexColours = mUseVertexColours;
		ribbonTrailRenderer->mMaxChainElements = mMaxChainElements;
		ribbonTrailRenderer->mTrailLength = mTrailLength;
		ribbonTrailRenderer->mSetLength = mSetLength;
		ribbonTrailRenderer->mTrailWidth = mTrailWidth;
		ribbonTrailRenderer->mSetWidth = mSetWidth;
		ribbonTrailRenderer->mRandomInitialColour = mRandomInitialColour;
		ribbonTrailRenderer->mInitialColour = mInitialColour;
		ribbonTrailRenderer->mColourChange = mColourChange;
	}

}
