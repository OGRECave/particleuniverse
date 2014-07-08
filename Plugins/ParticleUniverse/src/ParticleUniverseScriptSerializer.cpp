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

#include "ParticleUniversePCH.h"

#ifndef PARTICLE_UNIVERSE_EXPORTS
#define PARTICLE_UNIVERSE_EXPORTS
#endif

#include <iostream>

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	ParticleScriptSerializer::ParticleScriptSerializer(void) :
		mPath(BLANK_STRING),
		mIndentation(0),
		mKeyword(BLANK_STRING),
		mStreamToFile(true),
		mScriptString(BLANK_STRING)
	{
		mTab[0] = 0;
		mTab[1] = 48;
		mTab[2] = 52;
		mTab[3] = 56;
		mTab[4] = 60;
	}
	//-----------------------------------------------------------------------
	void ParticleScriptSerializer::writeScript(const ParticleSystem* particleSystem, const String& fileName)
	{
		// File open
		mStreamToFile = true;
		mScriptFile.open ((mPath + fileName).c_str());

		// Write
		mParticleSystemWriter.write(this, particleSystem);

		// File close
		mScriptFile.close();
	}
	//-----------------------------------------------------------------------
	const String& ParticleScriptSerializer::writeScript(const ParticleSystem* particleSystem)
	{
		// Write
		mStreamToFile = false;
		mScript.clear();
		mScript.str("");
		mParticleSystemWriter.write(this, particleSystem);
		mScriptString = mScript.str();
		return mScriptString;
	}
	//-----------------------------------------------------------------------
	void ParticleScriptSerializer::setDefaultTabs (
		short tab0, 
		short tab1, 
		short tab2, 
		short tab3,
		short tab4)
	{
	}
	//-----------------------------------------------------------------------
	void ParticleScriptSerializer::setPath (const String& path)
	{
		mPath = path;
	}
	//-----------------------------------------------------------------------
	void ParticleScriptSerializer::writeLine(
		const String& s0, 
		const String& s1, 
		const String& s2,
		const String& s3,
		const String& s4,
		short indentation0,
		short indentation1,
		short indentation2,
		short indentation3,
		short indentation4)
	{
		String base = BLANK_STRING;
		base.reserve(256);
		_appendTabbedString(0, base, s0, indentation0);
		_appendTabbedString(1, base, s1, indentation1);
		_appendTabbedString(2, base, s2, indentation2);
		_appendTabbedString(3, base, s3, indentation3);
		_appendTabbedString(4, base, s4, indentation4);
		base += "\n";
		_stream(base);
	}
	//-----------------------------------------------------------------------
	void ParticleScriptSerializer::writeLine(
		const String& s0, 
		const String& s1, 
		const String& s2,
		const String& s3,
		short indentation0,
		short indentation1,
		short indentation2,
		short indentation3)
	{
		String base = BLANK_STRING;
		_appendTabbedString(0, base, s0, indentation0);
		_appendTabbedString(1, base, s1, indentation1);
		_appendTabbedString(2, base, s2, indentation2);
		_appendTabbedString(3, base, s3, indentation3);
		base += "\n";
		_stream(base);
	}
	//-----------------------------------------------------------------------
	void ParticleScriptSerializer::writeLine(
		const String& s0, 
		const String& s1, 
		const String& s2,
		short indentation0,
		short indentation1,
		short indentation2)
	{
		String base = BLANK_STRING;
		_appendTabbedString(0, base, s0, indentation0);
		_appendTabbedString(1, base, s1, indentation1);
		_appendTabbedString(2, base, s2, indentation2);
		base += "\n";
		_stream(base);
	}
	//-----------------------------------------------------------------------
	void ParticleScriptSerializer::writeLine(
		const String& s0, 
		const String& s1,
		short indentation0,
		short indentation1)
	{
		String base = BLANK_STRING;
		_appendTabbedString(0, base, s0, indentation0);
		_appendTabbedString(1, base, s1, indentation1);
		base += "\n";
		_stream(base);
	}
	//-----------------------------------------------------------------------
	void ParticleScriptSerializer::writeLine(
		const String& s0,
		short indentation0)
	{
		String base = BLANK_STRING;
		_appendTabbedString(0, base, s0, indentation0);
		base += "\n";
		_stream(base);
	}
	//-----------------------------------------------------------------------
	void ParticleScriptSerializer::_appendTabbedString(
		size_t index, 
		String& base, 
		const String& s, 
		short indentation)
	{
		if (indentation > 0)
		{
			// Append gives garbage, so use for-loop instead
			for (short i = 0; i < indentation; ++i) base += " ";
		}
		else if (indentation < 0)
		{
			short pos = mTab[index] - (short)(base.length());
			pos = pos > 0 ? pos : 1;
			pos = pos < 0 ? 1 : pos;
			for (short i = 0; i < pos; ++i) base += " ";
		}

		base.append(s);
	}
	//-----------------------------------------------------------------------
	String ParticleScriptSerializer::toString(vector<Real> vector, bool applySqrt)
	{
    StringStream stream;
		if (!vector.empty())
		{
			for (size_t i = 0; i < vector.size(); ++i)
			{
				if (applySqrt)
				{
					stream << Math::Sqrt(vector[i]) << " ";
				}
				else
				{
					stream << vector[i] << " ";
				}
			}
		}
        return stream.str();
	}
	//-----------------------------------------------------------------------
	const short ParticleScriptSerializer::getIndentation(void) const
	{
		return mIndentation;
	}
	//-----------------------------------------------------------------------
	void ParticleScriptSerializer::setIndentation(const short indentation)
	{
		mIndentation = indentation;
	}
	//-----------------------------------------------------------------------
	const String& ParticleScriptSerializer::getKeyword(void) const
	{
		return mKeyword;
	}
	//-----------------------------------------------------------------------
	void ParticleScriptSerializer::setKeyword(const String& keyword)
	{
		mKeyword = keyword;
	}
	//-----------------------------------------------------------------------
	void ParticleScriptSerializer::_stream (const String& string)
	{
		if (mStreamToFile)
		{
			mScriptFile << string;
		}
		else
		{
			mScript << string;
		}
	}

}
