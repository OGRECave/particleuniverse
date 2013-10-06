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

#ifndef __PU_PHYSX_MATH_H__
#define __PU_PHYSX_MATH_H__

#include "ParticleUniversePrerequisites.h"
#ifdef PU_PHYSICS_PHYSX

	#include "NxVec3.h"
	#include "NxQuat.h"
	#include "Nxp.h"

	namespace ParticleUniverse
	{
		/** 
		*/
		class _ParticleUniverseExport PhysXMath
		{
			public:
				PhysXMath(void) {};
				virtual ~PhysXMath(void) {};

				// Converters
				static inline Vector3 convert(const NxVec3& nxVec3){ return Vector3(nxVec3.x, nxVec3.y, nxVec3.z); }
				static inline NxVec3 convert(const Vector3& vector3){ return NxVec3(vector3.x, vector3.y, vector3.z); }
				static inline Quaternion convert(const NxQuat& q) {return Quaternion(q.w, q.x, q.y, q.z); }
				static inline NxQuat convert(Quaternion& q, bool _normalise = true)
				{
					if (_normalise)
						q.normalise();
		
					NxQuat a;
					a.x = q.x;
					a.y = q.y;
					a.z = q.z;
					a.w = q.w;
					return a;
				}
				static inline GroupsMask convert(const NxGroupsMask& nxGroupsMask)
				{
					GroupsMask groupsMask;
					groupsMask.bits0 = nxGroupsMask.bits0;
					groupsMask.bits1 = nxGroupsMask.bits1;
					groupsMask.bits2 = nxGroupsMask.bits2;
					groupsMask.bits3 = nxGroupsMask.bits3;
					return groupsMask;
				}
				static inline NxGroupsMask convert(const GroupsMask& groupsMask)
				{
					NxGroupsMask nxGroupsMask;
					nxGroupsMask.bits0 = groupsMask.bits0;
					nxGroupsMask.bits1 = groupsMask.bits1;
					nxGroupsMask.bits2 = groupsMask.bits2;
					nxGroupsMask.bits3 = groupsMask.bits3;
					return nxGroupsMask;
				}
		};
	}
#endif //PU_PHYSICS_PHYSX
#endif