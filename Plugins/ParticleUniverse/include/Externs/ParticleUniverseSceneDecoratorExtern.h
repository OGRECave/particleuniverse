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

#ifndef __PU_SCENE_DECORATOR_EXTERN_H__
#define __PU_SCENE_DECORATOR_EXTERN_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseExtern.h"

namespace ParticleUniverse
{
	/** The SceneDecoratorExtern is a test class that can be used to add additional objects to the scene. This allows quick
		prototyping, but it it not really usable in a real situation.
    */
	class _ParticleUniverseExport SceneDecoratorExtern : public Extern
	{
		public:
			SceneDecoratorExtern(void);
	        virtual ~SceneDecoratorExtern(void) {};

			/** see Extern::_prepare
			*/
			virtual void _prepare(ParticleTechnique* technique);

			/** see Extern::_unprepare
			*/
			virtual void _unprepare(ParticleTechnique* technique);

			/** see Extern::_interface
			*/
			virtual void _interface(ParticleTechnique* technique, 
				Particle* particle, 
				Real timeElapsed);

			/** Create the desired entity
	        */
			void createEntity(void);

			/** Destroy the entity again
	        */
			void destroyEntity(void);

			/** Return the name of the mesh
	        */
			const String& getMeshName(void) const;

			/** Set the name of the mesh
	        */
			void setMeshName(const String& meshName);

			/** Return the name of the material used for the entity.
	        */
			const String& getMaterialName(void) const;

			/** Set the name of the material used for the entity.
	        */
			void setMaterialName(const String& materialName);

			/** Returns the scale value to which the node must be set.
	        */
			const Vector3& getScale(void) const;

			/** Sets the scale value to which the node must be set.
	        */
			void setScale(Vector3& scale);

			/** Returns the position value to which the node must be set.
	        */
			const Vector3& getPosition(void) const;

			/** Sets the position value to which the node must be set.
	        */
			void setPosition(Vector3& position);

			/** See Extern::_notifyStart.
			*/
			virtual void _notifyStart (void);

			/** See Extern::_notifyStop.
			*/
			virtual void _notifyStop (void);

			/** See Extern:copyAttributesTo
	        */
			virtual void copyAttributesTo (Extern* externObject);

		protected:

			bool mEntitySet;
			Ogre::Entity* mEntity;
			String mMeshName;
			String mEntityName;
			String mMaterialName;
			bool mMaterialNameSet;
			Vector3 mScale;
			Vector3 mPosition;
			Ogre::SceneNode* mSubnode;
			unsigned int mCount;

	};

}
#endif
