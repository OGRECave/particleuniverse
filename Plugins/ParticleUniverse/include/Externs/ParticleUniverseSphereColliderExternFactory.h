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

#ifndef __PU_SPHERE_COLLIDER_EXTERN_FACTORY_H__
#define __PU_SPHERE_COLLIDER_EXTERN_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseExternFactory.h"
#include "ParticleUniverseSphereColliderExtern.h"
#include "ParticleUniverseSphereColliderExternTokens.h"

namespace ParticleUniverse
{
	/** Factory class responsible for creating the SphereColliderExtern.
    */
	class _ParticleUniverseExport SphereColliderExternFactory : public ExternFactory
	{
		public:
			SphereColliderExternFactory(void) : 
				ExternFactory(){};
	        virtual ~SphereColliderExternFactory(void){};

			/** 
			*/
			String getExternType(void) const
			{
				return "SphereCollider";
			};

			/** 
			*/
			Extern* createExtern(void)
			{
				return _createExtern<SphereColliderExtern>();
			};

			/** See ScriptReader */
			virtual bool translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mSphereColliderExternTranslator.translateChildProperty(compiler, node);
			};

			/** See ScriptReader */
			virtual bool translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mSphereColliderExternTranslator.translateChildObject(compiler, node);
			};

			/*  */
			virtual void write(ParticleScriptSerializer* serializer , const IElement* element)
			{
				// Delegate to mSphereColliderExternWriter
				mSphereColliderExternWriter.write(serializer, element);
			}

		protected:
			SphereColliderExternWriter mSphereColliderExternWriter;
			SphereColliderExternTranslator mSphereColliderExternTranslator;
	};

}
#endif
