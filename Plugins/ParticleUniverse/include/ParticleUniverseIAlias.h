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

#ifndef __PU_I_ALIAS_H__
#define __PU_I_ALIAS_H__

#include "ParticleUniversePrerequisites.h"

namespace ParticleUniverse
{
	/** Abstract (pure virtual) alias class
	@remarks
		The IAlias class acts as an interface and is used to represent all other classes that are a child of the
		IAlias class. This typically concerns entities that are used in the Particle Universe scripts and for
		which it is possible to define an alias (i.e. ParticleTechnique, ParticleEmitter and ParticleAffector).
	*/
	class _ParticleUniverseExport IAlias
	{
		public:
			enum AliasType
			{
				AT_UNDEFINED,
				AT_TECHNIQUE,
				AT_RENDERER,
				AT_EMITTER,
				AT_AFFECTOR,
				AT_OBSERVER,
				AT_EXTERN,
				AT_HANDLER,
				AT_BEHAVIOUR
			};

			IAlias(void) : 
				mAliasName(BLANK_STRING),
				mAliasType(AT_UNDEFINED) {};
			virtual ~IAlias(void) {};

			/** 
			*/
			const String& getAliasName(void) const {return mAliasName;};
			void setAliasName(String aliasName) {mAliasName = aliasName;};

			/** 
			*/
			const AliasType getAliasType(void) const {return mAliasType;};
			void setAliasType(AliasType aliasType) {mAliasType = aliasType;};

		protected:
			String mAliasName;
			AliasType mAliasType;
	};

}
#endif
