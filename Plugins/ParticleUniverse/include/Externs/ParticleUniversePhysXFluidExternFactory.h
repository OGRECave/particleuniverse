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
Special thanks to: M. Geissler
-----------------------------------------------------------------------------------------------
*/

#ifndef __PU_PHYSX_FLUID_EXTERN_FACTORY_H__
#define __PU_PHYSX_FLUID_EXTERN_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#ifdef PU_PHYSICS_PHYSX

	#include "ParticleUniverseExternFactory.h"
	#include "ParticleUniversePhysXFluidExtern.h"
	#include "ParticleUniversePhysXFluidExternTokens.h"

	namespace ParticleUniverse
	{
		/** Factory class responsible for creating the PhysXFluidExtern.
		*/
		class _ParticleUniverseExport PhysXFluidExternFactory : public ExternFactory
		{
			public:
				PhysXFluidExternFactory(void) : 
					ExternFactory(){};
				virtual ~PhysXFluidExternFactory(void){};

				/** 
				*/
				String getExternType(void) const
				{
					return "PhysXFluid";
				};

				/** 
				*/
				Extern* createExtern(void)
				{
					return _createExtern<PhysXFluidExtern>();
				};

				/** See ScriptReader */
				virtual bool translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
				{
					return mPhysXFluidExternTranslator.translateChildProperty(compiler, node);
				};

				/** See ScriptReader */
				virtual bool translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
				{
					return mPhysXFluidExternTranslator.translateChildObject(compiler, node);
				};

				/*  */
				virtual void write(ParticleScriptSerializer* serializer , const IElement* element)
				{
					// Delegate to mPhysActorXExternWriter
					mPhysXFluidExternWriter.write(serializer, element);
				};

			protected:
				PhysXFluidExternWriter mPhysXFluidExternWriter;
				PhysXFluidExternTranslator mPhysXFluidExternTranslator;
		};

	}
#endif //PU_PHYSICS_PHYSX
#endif
