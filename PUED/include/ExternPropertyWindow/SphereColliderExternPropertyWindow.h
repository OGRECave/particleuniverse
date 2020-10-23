/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2014 Henry van Merode. All rights reserved.

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

#ifndef __PUED_SPHERE_COLLIDER_EXTERN_PROPERTY_WINDOW_H__
#define __PUED_SPHERE_COLLIDER_EXTERN_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseExternPropertyWindow.h"
#include "Externs/ParticleUniverseSphereColliderExtern.h"

/**	
*/
class SphereColliderExternPropertyWindow : public ExternPropertyWindow
{
	public:
		/**
			Constructor: Pass a parent window, the owner component and name
		*/
		SphereColliderExternPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);

		/**
			Constructor: Pass an extern window; its settings will be copied.
		*/
		SphereColliderExternPropertyWindow(ExternPropertyWindow* externPropertyWindow);

		/**
			Constructor: Pass an extern; its settings will be copied.
		*/
		SphereColliderExternPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::SphereColliderExtern* externObject);

		/**
			Destructor
		*/
		~SphereColliderExternPropertyWindow(void){};

		/**
			See ExternPropertyWindow
		*/
		virtual void copyAttributeToExtern(wxPGProperty* prop, wxString propertyName);
		
		/**
			See ExternPropertyWindow
		*/
		virtual void copyAttributesFromExtern(ParticleUniverse::Extern* externObject);

	protected:
		wxArrayString mIntersectionTypes;
		wxArrayString mCollisionTypes;

		/**
			Initialises all wphere extern properties
		*/
		virtual void _initProperties(void);

};

#endif

