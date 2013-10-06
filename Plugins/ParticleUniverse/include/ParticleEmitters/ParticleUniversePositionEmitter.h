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

#ifndef __PU_POSITION_EMITTER_H__
#define __PU_POSITION_EMITTER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseEmitter.h"

namespace ParticleUniverse
{
	/** The PositionEmitter is an emitter that emits particles from one or more given positions.
		The PositionEmitter enables building up a predefined structure out of particles. Imaging building up a
		wall that is made up from stone blocks. The particles are emitted on the positions that are added to this
		emitter. Adding some physical behaviour to the particles and you have your stone wall that collapses if a 
		force is applied to it.
    */
	class _ParticleUniverseExport PositionEmitter : public ParticleEmitter
	{
		protected:
			vector<Vector3> mPositionList;
			bool mRandomized;
			size_t mIndex;

		public:
			// Constants
			static const bool DEFAULT_RANDOMIZE;

			PositionEmitter(void);
	        virtual ~PositionEmitter(void) {};

			/** 
	        */
			bool isRandomized(void) const;

			/** 
	        */
			void setRandomized(bool randomized);

			/** Returns a list with positions
	        */
			const vector<Vector3>& getPositions(void) const;

			/** Add a new position to this emitter
	        */
			void addPosition(const Vector3& position);

			/** Remove all positions from this emitter
	        */
			void removeAllPositions(void);

			/** See ParticleEmitter
			*/
			void _notifyStart(void);

			/** See ParticleEmitter
			*/
			virtual unsigned short _calculateRequestedParticles(Real timeElapsed);

			/** Generate a particle position based on the added positions.
	        */
			virtual void _initParticlePosition(Particle* particle);

			/** 
	        */
			virtual void copyAttributesTo (ParticleEmitter* emitter);
	};

}
#endif
