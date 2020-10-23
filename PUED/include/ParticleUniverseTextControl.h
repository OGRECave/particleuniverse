/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_TEXTCONTROL_H__
#define __PUED_TEXTCONTROL_H__

#include "wx/ogre/prerequisites.h"

class ParticleUniverseEditorFrame;
class TextControl : public wxTextCtrl
{
	public:
		TextControl(ParticleUniverseEditorFrame* frame, wxWindow *parent, wxWindowID id,
               const wxString& value = wxT(""),
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxValidator& validator = wxDefaultValidator,
			   const wxString& name = wxTextCtrlNameStr);
		virtual ~TextControl(){};
		
		/**
			Detect that the script has been changed
		*/
		void OnTextUpdated(wxCommandEvent& evt);

		/**
			Detect key events
		*/
		void OnKeyPressed(wxKeyEvent& event);

		/**
			Add a text that needs to be highlighted in a certain colour
		*/
		void addHighLightedText(wxString, wxTextAttr);

		/**
			Check, whether string in the text - defined by location 'from' and 'to', is a keyword.
			If that is the case, highlight it
		*/
		void checkKeyword(long from, long to);

		/**
			The same as checkKeyword, but now for multiple words in a text defined by location 'from' and 'to'.
		*/
		void checkKeywords(long from, long to, bool freeze = true);

		/**
			Override wxTextCtrl
		*/
		virtual void AppendText(const wxString& text);

		/**
			Get/Set flag to highlight the keywords
		*/
		void setHighlightKeywords(bool highlightKeywords);
		bool getHighlightKeywords(void) const;

	protected:
		ParticleUniverseEditorFrame* mFrame;
		std::map<wxString, wxTextAttr> mHighlightedText;
		bool mHighlightKeywords;
		bool mPaste;

		/**
			Check, wheter a position in the text is on a space or eol character
		*/
		bool validChar(long pos);

		/**
			Determine the word just before a position
		*/
		void findWordToLeft(long insert, long& from, long& to);

		/**
			Determine the word just after a position
		*/
		void findWordToRight(long insert, long& from, long& to);
};

#endif
