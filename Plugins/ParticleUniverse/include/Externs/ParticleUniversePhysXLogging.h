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

#ifndef __PU_PHYSX_LOGGING_H__
#define __PU_PHYSX_LOGGING_H__

#include "ParticleUniversePrerequisites.h"
#ifdef PU_PHYSICS_PHYSX

	namespace ParticleUniverse
	{
		class PhysXLogging : public NxUserOutputStream
		{
			public:
				/** 
				*/
				void reportError(NxErrorCode e, const char* message, const char* file, int line)
				{
					LogManager::getSingleton().logMessage("ParticleUniverse PhysX: " + 
						String(file) + " " + 
						StringConverter::toString(line));
					switch (e)
					{
						case NXE_INVALID_PARAMETER:
						LogManager::getSingleton().logMessage("Invalid parameter");
						break;

						case NXE_INVALID_OPERATION:
						LogManager::getSingleton().logMessage("Invalid operation");
						break;

						case NXE_OUT_OF_MEMORY:
						LogManager::getSingleton().logMessage("Out of memory");
						break;

						case NXE_DB_INFO:
						LogManager::getSingleton().logMessage("Info");
						break;

						case NXE_DB_WARNING:
						LogManager::getSingleton().logMessage("Warning");
						break;

						default:
						LogManager::getSingleton().logMessage("Unknown error");
					}

					LogManager::getSingleton().logMessage(String(message));
				}

				/** 
				*/
				NxAssertResponse reportAssertViolation(const char* message, const char* file, int line)
				{
					LogManager::getSingleton().logMessage("Particle Universe PhysX - Access Violation: " + 
						String(message) + " " + 
						String(file) + " " + 
						StringConverter::toString(line));
	#ifdef WIN32
					switch (MessageBox(0, message, "AssertViolation, see console for details.", MB_ABORTRETRYIGNORE))
					{
						case IDRETRY:
							return NX_AR_CONTINUE;
						case IDIGNORE:
							return NX_AR_IGNORE;
						case IDABORT:
						default:
						return NX_AR_BREAKPOINT;
					}
	#elif LINUX
					assert(0);
	#elif _XBOX
					return NX_AR_BREAKPOINT;
	#elif __CELLOS_LV2__
					return NX_AR_BREAKPOINT;
	#endif
				}

				/** 
				*/
				void print(const char* message)
				{
					LogManager::getSingleton().logMessage(String(message));
				}
		};

	}
#endif //PU_PHYSICS_PHYSX
#endif