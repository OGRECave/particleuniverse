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

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseTextControl.h"
#include "ParticleUniverseEditor.h"

//-----------------------------------------------------------------------
TextControl::TextControl(ParticleUniverseEditorFrame* frame, wxWindow *parent, wxWindowID id,
               const wxString& value,
               const wxPoint& pos,
               const wxSize& size,
               long style,
               const wxValidator& validator,
			   const wxString& name) : 
	wxTextCtrl(parent, id, value, pos, size, style, validator, name),
	mFrame(frame),
	mHighlightKeywords(true),
	mPaste(false)
{
	addHighLightedText(wxT("system"), wxTextAttr(wxT("#151B8D")));
	addHighLightedText(wxT("technique"), wxTextAttr(wxT("#6698FF")));
	addHighLightedText(wxT("renderer"), wxTextAttr(wxT("#E42217")));
	addHighLightedText(wxT("emitter"), wxTextAttr(wxT("#4CC417")));
	addHighLightedText(wxT("affector"), wxTextAttr(wxT("#FBB117")));
	addHighLightedText(wxT("observer"), wxTextAttr(wxT("#254117")));
	addHighLightedText(wxT("handler"), wxTextAttr(wxT("#8D38C9")));
	addHighLightedText(wxT("behaviour"), wxTextAttr(wxT("#307D7E")));
	addHighLightedText(wxT("extern"), wxTextAttr(wxT("#827B60")));

	Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(TextControl::OnTextUpdated));
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(TextControl::OnKeyPressed));
};
//-----------------------------------------------------------------------
void TextControl::addHighLightedText(wxString str, wxTextAttr attr)
{
	wxFont font = wxFont(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	attr.SetFont(font);
	mHighlightedText[str] = attr;
}
//-----------------------------------------------------------------------
void TextControl::OnTextUpdated(wxCommandEvent& evt)
{
	if (IsFrozen())
		return;

	mFrame->notifyScriptChanged();

	if (!mHighlightKeywords)
		return;

	if (mPaste)
	{
		long insert = GetInsertionPoint() == GetLastPosition() ? GetLastPosition() : GetInsertionPoint() + 1;

		// First reset to black
		wxTextAttr textAttr = wxTextAttr(*wxBLACK);
		wxFont font = wxFont(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		textAttr.SetFont(font);
		SetStyle(0, insert, textAttr);

		// Then check for keywords
		checkKeywords(0, insert, false);
		mPaste = false;
		return;
	}

	// Set the colours
	long insert = GetInsertionPoint();
	long x = 0;
	long line = 0;
	{
		long from = 0;
		long to = 0;
		long l = 0;

		// Check the location of the insertion point in relation to the word
		if (insert > 1 && insert < GetLastPosition() && validChar(insert) && !validChar(insert-1) && validChar(insert-2))
		{
			// 1. Insertion is between words, separated by 1 non-character
			findWordToLeft(insert-1, from, to);
			checkKeyword(from, to);

			findWordToRight(insert, from, to);
			checkKeyword(from, to);
		}
		else if (insert > 0 && validChar(insert) && validChar(insert-1))
		{
			// 2. Insertion point is in a word

			// Get the left part of the word
			l = insert;
			while (l >= 0 && validChar(l))
			{
				l--;
			}
			from = l+1;

			// Get the right part of the word
			l = insert;
			while (l < GetLastPosition() && validChar(l))
			{
				l++;
			}
			to = l;
			checkKeyword(from, to);
		}
		else if (validChar(insert))
		{
			// 3. Insertion point is before a word
			findWordToRight(insert, from, to);
			checkKeyword(from, to);
		}
		else if (insert > 0 && validChar(insert-1))
		{
			// 4. Insertion point is after a word
			findWordToLeft(insert, from, to);
			checkKeyword(from, to);
		}
	}
}
//-----------------------------------------------------------------------
void TextControl::OnKeyPressed(wxKeyEvent& event)
{
	event.Skip();
	if ((event.GetKeyCode() == 86 || event.GetKeyCode() == 118) && event.GetModifiers() == wxMOD_CONTROL)
	{
		// CTRL-V => Paste
		mPaste = true;
	}
	else if (event.GetKeyCode() == WXK_INSERT && event.GetModifiers() == wxMOD_SHIFT)
	{
		// SHIFT-INSERT => Paste
		mPaste = true;
	}
}
//-----------------------------------------------------------------------
void TextControl::checkKeywords(long from, long to, bool freeze)
{
	if (freeze)
	{
		Freeze();
	}
	long l = from;
	while (l < to)
	{
		if (validChar(l))
		{
			// Start word
			long startWord = l;
			while (l < to)
			{
				if (!validChar(l))
				{
					// End word
					checkKeyword(startWord, l);
					break;
				}
				l++;
			}
		}
		l++;
	}
	if (freeze)
	{
		Thaw();
	}
}
//-----------------------------------------------------------------------
void TextControl::checkKeyword(long from, long to)
{
	bool keyword = false;
	wxString text = GetRange(from, to);
	std::map<wxString, wxTextAttr>::iterator it;
	for (it = mHighlightedText.begin(); it != mHighlightedText.end(); ++it)
	{
		wxString compare = it->first;
		if (text == compare)
		{
			SetStyle(from, to, it->second);
			keyword = true;
		}
	}
	if (!keyword)
	{
		wxTextAttr textAttr = wxTextAttr(*wxBLACK);
		wxFont font = wxFont(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		textAttr.SetFont(font);
		SetStyle(from, to, textAttr);
	}
}
//-----------------------------------------------------------------------
void TextControl::AppendText(const wxString& text)
{
	wxTextCtrl::AppendText(text);
	if (mHighlightKeywords)
	{
		checkKeywords(0, GetLastPosition());
	}
}
//-----------------------------------------------------------------------
bool TextControl::validChar(long pos)
{
	wxString text = GetRange(pos, pos + 1);
	return !(text == wxT(" ") || text == wxT("\n"));
}
//-----------------------------------------------------------------------
void TextControl::findWordToRight(long insert, long& from, long& to)
{
	from = insert;
	long l = insert + 1;
	while (l < GetLastPosition() && validChar(l))
	{
		l++;
	}
	to = l;
}
//-----------------------------------------------------------------------
void TextControl::findWordToLeft(long insert, long& from, long& to)
{
	long l = insert - 1;
	while (l >= 0 && validChar(l))
	{
		l--;
	}
	from = l + 1;
	to = insert;
}
//-----------------------------------------------------------------------
void TextControl::setHighlightKeywords(bool highlightKeywords)
{
	mHighlightKeywords = highlightKeywords;
	if (highlightKeywords)
	{
		checkKeywords(0, GetLastPosition());
	}
	else
	{
		wxTextAttr textAttr = wxTextAttr(*wxBLACK);
		wxFont font = wxFont(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		textAttr.SetFont(font);
		SetStyle(0, GetLastPosition(), textAttr);
	}
}
//-----------------------------------------------------------------------
bool TextControl::getHighlightKeywords(void) const
{
	return mHighlightKeywords;
}