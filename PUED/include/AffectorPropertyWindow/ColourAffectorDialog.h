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

#ifndef __PUED_COLOUR_AFFECTOR_DIALOG_H__
#define __PUED_COLOUR_AFFECTOR_DIALOG_H__

#include "ParticleUniverseControlPointColourControl.h"
#include <wx/dialog.h>

//#define ID_RESET 1999

/**	Defining time + colour for the ColourAffector
*/
class ColourAffectorDialog : public wxDialog
{
	public:
		// Constructor / Destructor
		ColourAffectorDialog(ParticleUniverse::ColourAffector::ColourMap& map, 
			wxWindow* parent, 
			wxWindowID id, 
			const wxString& title, 
			const wxPoint& pos = wxDefaultPosition, 
			const wxSize& size = wxDefaultSize, 
			const wxString& name = wxT("ColourAffectorDialog"));
		~ColourAffectorDialog(void){};
		void OnReset(wxCommandEvent& event);
		bool Destroy();
		ParticleUniverse::ColourAffector::ColourMap& getColourMap(void);

	protected:
		ControlPointColourControl* mControlPointColourControl;
		ParticleUniverse::ColourAffector::ColourMap mColourMap;
		std::vector<ControlPoint*> mControlPoints;
		wxSize mControlSize;
};

#endif
