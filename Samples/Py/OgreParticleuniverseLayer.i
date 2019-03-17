%module(directors="1") OgrePULayer
%{
#include "Ogre.h"
#include "ParticleUniverseSystemManager.h"
#include "OgreParticleuniverseLayer.h"
%}

%include std_string.i
%include exception.i
%include stdint.i
%include typemaps.i
%import "Ogre.i"
%import "Bites/OgreBites.i"

%apply bool* INOUT { bool* p_open };
%apply float* INOUT { float* v };
%apply int* INOUT { int* v };


#ifdef SWIGPYTHON
%pythoncode
%{
    __version__ = "0.1"
%}
#endif

%include "OgreParticleuniverseLayer.h"