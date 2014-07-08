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

#ifndef __PU_PARTICLE_SCRIPT_SERIALIZER_H__
#define __PU_PARTICLE_SCRIPT_SERIALIZER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemTokens.h"
#include "ParticleUniverseContext.h"

namespace ParticleUniverse
{
	/** The ParticleScriptSerializer class is responsible for writing objects and attributes to a particle system script.
	*/
	class _ParticleUniverseExport ParticleScriptSerializer : public SerializerAlloc
	{
		public:
			Context context;
			ParticleScriptSerializer(void);
			virtual ~ParticleScriptSerializer(void) {};

			/** Writes a ParticleSystem to a file in the script format.
			*/
			void writeScript(const ParticleSystem* particleSystem, const String& fileName);

			/** If no filename has been given, serialisation is done to a string.
			*/
			const String& writeScript(const ParticleSystem* particleSystem);

			/** Writes a line to a script file.
			*/
			void writeLine(
				const String& s0, 
				const String& s1, 
				const String& s2,
				const String& s3,
				const String& s4,
				short indentation0 = -1,
				short indentation1 = -1,
				short indentation2 = -1,
				short indentation3 = -1,
				short indentation4 = -1);

			/** Writes a line to a script file.
			*/
			void writeLine(
				const String& s0, 
				const String& s1, 
				const String& s2,
				const String& s3,
				short indentation0 = -1,
				short indentation1 = -1,
				short indentation2 = -1,
				short indentation3 = -1);

			/** Writes a line to a script file.
			*/
			void writeLine(
				const String& s0, 
				const String& s1, 
				const String& s2,
				short indentation0 = -1,
				short indentation1 = -1,
				short indentation2 = -1);

			/** Writes a line to a script file.
			*/
			void writeLine(
				const String& s0, 
				const String& s1,
				short indentation0 = -1,
				short indentation1 = -1);

			/** Writes a line to a script file.
			*/
			void writeLine(
				const String& s0,
				short indentation0 = -1);

			/** Set the default tab values. If a writeline doesn't contain any indentation value, the default tab values are 
				used.
			*/
			void setDefaultTabs (
				short tab0 = 0, 
				short tab1 = 48, 
				short tab2 = 52, 
				short tab3 = 56,
				short tab4 = 60);

			/** Set the directory to which files are serialized.
			*/
			void setPath (const String& path);

			/** Creates a string from a vector with Reals. This function contains an additional argument. If set to true, the
				square root is applied to all real values (don't ask!).
			*/
			static String toString(vector<Real> vector, bool applySqrt = false);

			/** Get/set the indentation. To be used in cases where it is unkown what the indentation is.
			*/
			const short getIndentation(void) const;
			void setIndentation(const short indentation);

			/** Get/set the keyword. To be used in cases where it is unkown what the keyword is or in case serialisation is spread across
				multiple objects.
			*/
			const String& getKeyword(void) const;
			void setKeyword(const String& keyword);

		protected:
			ParticleSystemWriter mParticleSystemWriter;
			String mPath;
			std::ofstream mScriptFile;
			StringStream mScript;
			String mScriptString;
			short mTab[5];
			short mIndentation;
			String mKeyword;
			bool mStreamToFile;

			/** Concatenate base string and string 's', with the proper indentation.
			*/
			void _appendTabbedString(
				size_t index, 
				String& base, 
				const String& s, 
				short indentation);

			/** Writes to a file or to a string
			*/
			void _stream (const String& string);
	};

}
#endif
