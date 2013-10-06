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

#ifndef __PU_I_DEPENDENCY_H__
#define __PU_I_DEPENDENCY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseIElement.h"

namespace ParticleUniverse
{
	/** Abstract (pure virtual) dependency class
	@remarks
		In some cases, an attribute of a ParticleSystem or its underlying components (ParticleEmitter, ...) may 
		depend on another value that changes over time. The ´changing value´ is wrapped into a IDependency class
		and the attribute is ´continuesly´ changed by the IDependency subclass.
	*/
	class _ParticleUniverseExport IDependency : public IElement
	{
		public:
			IDependency(void) : IElement() {};
			virtual ~IDependency(void){};
	};

}
#endif
