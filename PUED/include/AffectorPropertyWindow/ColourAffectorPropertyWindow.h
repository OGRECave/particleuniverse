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

#ifndef __PUED_COLOUR_AFFECTOR_PROPERTY_WINDOW_H__
#define __PUED_COLOUR_AFFECTOR_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseAffectorPropertyWindow.h"
#include "ParticleAffectors/ParticleUniverseColourAffector.h"

// Property names
static wxString PRNL_TIME_AND_COLOUR = wxT("");
static wxString PRNL_TIME_AND_COLOUR_PARENT = wxT("");
static wxString PRNL_TIME_AND_COLOUR_TIME = wxT("");
static wxString PRNL_TIME_AND_COLOUR_COLOUR = wxT("");
static wxString PRNL_COLOUR_OPERATION = wxT("");
static wxString COP_SET = wxT("");
static wxString COP_MULTIPLY = wxT("");

/**	Class that creates a time with colour property. After clicking, a new time / colour combination is added
*/
class ParentPropertyTimeAndColour : public wxParentPropertyClass
{
	public:
		ParentPropertyTimeAndColour(
			const wxString& label, 
			const wxString& name);
		virtual ~ParentPropertyTimeAndColour(void){};

		/**	
		*/
		virtual bool OnEvent (wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event);

		/**	Add the position
		*/
		//wxPGId* addTimeAndColour(wxPropertyGrid* propgrid, ParticleUniverse::Real time = 0.0f, wxColor colour = wxColour(255, 255, 255, 255));

		/**	Return the number of time/colour entries.
		*/
		int getNumberOfTimeAndColourEntries(void);

		/**	Returns the time value of the entry identified by index.
		*/
		//ParticleUniverse::Real getTime(wxPropertyGrid* propgrid, unsigned int index);

		/**	Returns the colour value of the entry identified by index.
		*/
		//const wxColor& getColour(wxPropertyGrid* propgrid, unsigned int index, wxColor& colour);

		/**	Removes all the child properties.
		*/
		void reset(void);

	protected:
		unsigned int mTimeAndColour;
};

/**	
*/
class ColourAffectorPropertyWindow : public AffectorPropertyWindow
{
	public:
		/**
			Constructor: Pass a parent window, the owner component and name
		*/
		ColourAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);

		/**
			Constructor: Pass an affector window; its settings will be copied.
		*/
		ColourAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow);

		/**
			Constructor: Pass an affector; its settings will be copied.
		*/
		ColourAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::ColourAffector* affector);

		/**
			Destructor
		*/
		~ColourAffectorPropertyWindow(void){};

		/**
			See AffectorPropertyWindow
		*/
		virtual void copyAttributeToAffector(wxPGProperty* prop, wxString propertyName);
		
		/**
			See AffectorPropertyWindow
		*/
		virtual void copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector);

	protected:
		wxArrayString mColourOperation;

		/**
			Initialises all box affector properties
		*/
		virtual void _initProperties(void);

};

#endif

