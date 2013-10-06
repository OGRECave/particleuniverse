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

#include "ParticleRenderers/ParticleUniverseSphereRenderer.h"
#include "ParticleRenderers/ParticleUniverseSphere.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	SphereRenderer::SphereRenderer(void) : ParticleRenderer()
	{
		// Create sphere set
		mSphereSet = PU_NEW SphereSet("", 0, true);

		// World-relative axes
		mSphereSet->setSpheresInWorldSpace(true);

		autoRotate = false;
	}
	//-----------------------------------------------------------------------
	SphereRenderer::~SphereRenderer(void)
	{
		if (mSphereSet)
		{
			PU_DELETE mSphereSet;
			mSphereSet = 0;
		}
	}
	//-----------------------------------------------------------------------
	void SphereRenderer::_prepare(ParticleTechnique* technique)
	{
		// Use the given technique, although it should be the same as mParentTechnique (must be set already)
		if (!technique || mRendererInitialised)
			return;

		_notifyParticleQuota(technique->getVisualParticleQuota());

		// Notify attached, but only if the parentnode exists
		if (technique->getParentSystem()->getParentNode())
		{		
			_notifyAttached(technique->getParentSystem()->getParentNode(), 
							technique->getParentSystem()->isParentIsTagPoint());
		}
		_notifyDefaultDimensions(_mRendererScale.x * technique->getDefaultWidth(),
								_mRendererScale.y * technique->getDefaultHeight(),
								_mRendererScale.z * technique->getDefaultDepth());
		_setMaterialName(technique->getMaterialName());
		mSphereSet->setRenderQueueGroup(mQueueId);
		mRendererInitialised = true;
	}
	//-----------------------------------------------------------------------
	void SphereRenderer::_unprepare(ParticleTechnique* technique)
	{
		_notifyAttached(0); // Bugfix V1.5: If detached from scenenode, do not use the pointer to it
	}
	//-----------------------------------------------------------------------
	void SphereRenderer::_updateRenderQueue(Ogre::RenderQueue* queue, ParticlePool* pool)
	{
		// Always perform this one
		ParticleRenderer::_updateRenderQueue(queue, pool);

		if (!mVisible)
			return;

		// Fast check to determine whether there are visual particles
		if (pool->isEmpty(Particle::PT_VISUAL))
			return;

		mSphereSet->setCullIndividually(mCullIndividual);

		mSphereSet->beginSpheres(pool->getSize(Particle::PT_VISUAL));
		Sphere sphere;
		
		VisualParticle* particle = static_cast<VisualParticle*>(pool->getFirst(Particle::PT_VISUAL));

		// Activate texture rotation if needed and use the speed of the first particle.
		// The implementation of the texture rotation is low-profile, because its value is limited. For 3d
		// particles the geometry rotation is more interesting.
		if (mSphereSet->isZRotated())
		{
			mSphereSet->rotateTexture(particle->zRotationSpeed.valueRadians());
		}

		// Loop through the particles
		while (!pool->end(Particle::PT_VISUAL))
		{
			if (particle)
			{
				sphere.mPosition = particle->position;
				sphere.mColour = particle->colour;
				sphere.mOrientation = particle->orientation;

				if (sphere.mOwnRadius = particle->ownDimensions)
				{
					sphere.mOwnRadius = true;
					sphere.setRadius (0.5f * particle->width);
				}
					
				mSphereSet->injectSphere(sphere);
			}
			
			particle = static_cast<VisualParticle*>(pool->getNext(Particle::PT_VISUAL));
		}

        mSphereSet->endSpheres();

		// Update the queue
		mSphereSet->_updateRenderQueue(queue);
	}
	//-----------------------------------------------------------------------
	void SphereRenderer::_notifyAttached(Ogre::Node* parent, bool isTagPoint)
	{
		mSphereSet->_notifyAttached(parent, isTagPoint);
	}
	//-----------------------------------------------------------------------
	void SphereRenderer::_setMaterialName(const String& materialName)
	{
		mSphereSet->setMaterialName(materialName);
	}
	//-----------------------------------------------------------------------
	void SphereRenderer::_notifyCurrentCamera(Camera* cam)
	{
		mSphereSet->_notifyCurrentCamera(cam);
	}
	//-----------------------------------------------------------------------
	void SphereRenderer::_notifyParticleQuota(size_t quota)
	{
		mSphereSet->setPoolSize(static_cast<unsigned int>(quota));
	}
	//-----------------------------------------------------------------------
	void SphereRenderer::_notifyDefaultDimensions(Real width, Real height, Real depth)
	{
		mSphereSet->setDefaultRadius(0.5f * width);
	}
	//-----------------------------------------------------------------------
	void SphereRenderer::_notifyParticleResized(void)
	{	
		mSphereSet->_notifyResized();
	}
	//-----------------------------------------------------------------------
	void SphereRenderer::_notifyParticleZRotated(void)
	{
		mSphereSet->_notifyZRotated();
	}
	//-----------------------------------------------------------------------
	void SphereRenderer::setRenderQueueGroup(uint8 queueId)
	{
		mQueueId = queueId;
		if (mSphereSet)
		{
			mSphereSet->setRenderQueueGroup(queueId);
		}
	}
	//-----------------------------------------------------------------------
	void SphereRenderer::setVisible(bool visible)
	{
		ParticleRenderer::setVisible(visible);
		mSphereSet->setVisible(visible);
	}
	//-----------------------------------------------------------------------
	void SphereRenderer::copyAttributesTo (ParticleRenderer* renderer)
	{
		// First copy parent attributes
		ParticleRenderer::copyAttributesTo(renderer);

		// First cast to SphereRenderer
		SphereRenderer* sphereRenderer = static_cast<SphereRenderer*>(renderer);

		// Copy attributes in case there is a SphereSet (which should be available)
		if (!sphereRenderer->getSphereSet())
			return;
	}

}
