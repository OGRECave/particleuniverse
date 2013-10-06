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

#ifndef __PU_BEAM_RENDERER_H__
#define __PU_BEAM_RENDERER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseRenderer.h"
#include "ParticleUniverseTechnique.h"
#include "ParticleUniverseTechniqueListener.h"
#include "OgreBillboardChain.h"

namespace ParticleUniverse
{
	/** Visual data specific for this type of renderer.
    */
	class _ParticleUniverseExport BeamRendererVisualData : public IVisualData
	{
		public:
			BeamRendererVisualData (size_t index, Ogre::BillboardChain* billboardChain) : 
				IVisualData(), 
				chainIndex(index),
				mTimeSinceLastUpdate(0.0f),
				mBillboardChain(billboardChain){};

			// Set the chain visible or invisible (PU 1.4)
			virtual void setVisible(bool visible){/* No implementation */};

			/** The is no decent way to make the individual chains/elements invisible. The width of each element is set to 0 to make it invisible.
				PU 1.4
			*/
			virtual void setVisible(bool visible, Real width)
			{
				if (!mBillboardChain)
					return;

				// Set width to 0 if not visible
				width = visible ? width : 0;
				
				size_t max = mBillboardChain->getMaxChainElements();
				Ogre::BillboardChain::Element element;
				for (size_t j = 0; j < max; j++)
				{
					Ogre::BillboardChain::Element element;
					element = mBillboardChain->getChainElement(chainIndex, j);
					element.width = width;
					mBillboardChain->updateChainElement(chainIndex, j, element);
				}
			}

			// Index of the chain
			size_t chainIndex;
			Vector3 half[100];
			Vector3 destinationHalf[100];
			Real mTimeSinceLastUpdate;
			Ogre::BillboardChain* mBillboardChain;
	};

	/** The BeamRenderer class is responsible to render particles as a BillboardChain.
    */
	class _ParticleUniverseExport BeamRenderer : public ParticleRenderer, public TechniqueListener
	{
		public:

			// Constants
			static const bool DEFAULT_USE_VERTEX_COLOURS;
			static const size_t DEFAULT_MAX_ELEMENTS;
			static const Real DEFAULT_UPDATE_INTERVAL;
			static const Real DEFAULT_DEVIATION;
			static const size_t DEFAULT_NUMBER_OF_SEGMENTS;
			static const Ogre::BillboardChain::TexCoordDirection DEFAULT_TEXTURE_DIRECTION;

			BeamRenderer(void);
	        virtual ~BeamRenderer(void);

			/** Getters and Setters
			*/
			bool isUseVertexColours(void) const;
			void setUseVertexColours(bool useVertexColours);

			size_t getMaxChainElements(void) const;
			void setMaxChainElements(size_t maxChainElements);

			Real getUpdateInterval(void) const;
			void setUpdateInterval(Real updateInterval);

			Real getDeviation(void) const;
			void setDeviation(Real deviation);

			size_t getNumberOfSegments(void) const;
			void setNumberOfSegments(size_t numberOfSegments);

			bool isJump(void) const;
			void setJump(bool jump);

			Ogre::BillboardChain::TexCoordDirection getTexCoordDirection(void) const;
			void setTexCoordDirection(Ogre::BillboardChain::TexCoordDirection texCoordDirection);

			/** @copydoc ParticleRenderer::_prepare */
			virtual void _prepare(ParticleTechnique* technique);

			/** @copydoc ParticleRenderer::_unprepare */
			virtual void _unprepare(ParticleTechnique* technique);

			/** Destroys the BillboarChain
			*/
			void _destroyAll(void);

			/** 
			*/
			virtual void _updateRenderQueue(Ogre::RenderQueue* queue, ParticlePool* pool);

			/** See ParticleRenderer
			*/
			inline virtual void _processParticle(ParticleTechnique* particleTechnique, 
				Particle* particle, 
				Real timeElapsed, 
				bool firstParticle);

			/** 
			*/
			virtual void _notifyAttached(Ogre::Node* parent, bool isTagPoint = false){/* No implementation here */};

			/** @copydoc ParticleRenderer::_setMaterialName */
			virtual void _setMaterialName(const String& materialName);

			/** 
			*/
			virtual void _notifyCurrentCamera(Camera* cam);
	
			/** 
			*/
			virtual void _notifyParticleQuota(size_t quota);

			/** 
			*/
			virtual void _notifyDefaultDimensions(Real width, Real height, Real depth){/* No implementation */};

			/** 
			*/
			virtual void _notifyParticleResized(void){/* No implementation */};

			/** 
			*/
			virtual void _notifyParticleZRotated(void){/* No implementation */};

			/** 
			*/
			virtual void setRenderQueueGroup(uint8 queueId);

			/** @copydoc ParticleRenderer::_getSortMode */
			virtual SortMode _getSortMode(void) const;

			/** @copydoc ParticleRenderer::setVisible */
			virtual void setVisible(bool visible);

			/** 
			*/
			virtual void copyAttributesTo (ParticleRenderer* renderer);

			/*  See TechniqueListener.
			*/
			virtual void particleEmitted(ParticleTechnique* particleTechnique, Particle* particle);

			/*  See TechniqueListener.
			*/
			virtual void particleExpired(ParticleTechnique* particleTechnique, Particle* particle);

		protected:

			String mBillboardChainName;
			Ogre::BillboardChain* mBillboardChain;
			vector<BeamRendererVisualData*> mAllVisualData;
			vector<BeamRendererVisualData*> mVisualData;
			size_t mQuota;
			bool mUseVertexColours;
			size_t mMaxChainElements;
			Real mUpdateInterval;
			Real mDeviation;
			size_t mNumberOfSegments;
			bool mJump;
			Ogre::BillboardChain::TexCoordDirection mTexCoordDirection;
	};

}
#endif
