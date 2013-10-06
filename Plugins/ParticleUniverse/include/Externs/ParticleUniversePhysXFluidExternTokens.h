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

#ifndef __PU_PHYSX_FLUID_EXTERN_TOKENS_H__
#define __PU_PHYSX_FLUID_EXTERN_TOKENS_H__

#include "ParticleUniversePrerequisites.h"
#ifdef PU_PHYSICS_PHYSX
	#include "ParticleUniverseExternTokens.h"

	namespace ParticleUniverse
	{
		/** The PhysXFluidExternTranslator parses 'PhysXFluidExtern' tokens
		*/
		class _ParticleUniverseExport PhysXFluidExternTranslator : public ScriptTranslator
		{
			public:
				PhysXFluidExternTranslator(void){};
				~PhysXFluidExternTranslator(void){};
				virtual bool translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node);
				virtual bool translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node);
		};
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------

		/** The PhysXFluidFlagTranslator also parses 'PhysXFluidExtern' tokens
		*/
		class _ParticleUniverseExport PhysXFluidFlagTranslator : public ScriptTranslator
		{
			public:
				PhysXFluidFlagTranslator(void){};
				~PhysXFluidFlagTranslator(void){};
				virtual void translate(ScriptCompiler* compiler, const AbstractNodePtr &node);
		};

		/** 
		*/
		class _ParticleUniverseExport PhysXFluidExternWriter : public ExternWriter
		{
			public:

				PhysXFluidExternWriter(void) {};
				virtual ~PhysXFluidExternWriter(void) {};

				/** @see
					ScriptWriter::write
				*/
				virtual void write(ParticleScriptSerializer* serializer , const IElement* element);
		};

	}
#endif //PU_PHYSICS_PHYSX
#endif
