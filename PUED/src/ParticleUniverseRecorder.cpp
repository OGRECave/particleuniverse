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
#include "ParticleUniverseRecorder.h"
#include "OgreMaterialManager.h"
#include "OgreRoot.h"
#include "OgreHardwarePixelBuffer.h"

//-----------------------------------------------------------------------
void ImageX::filterAlphaFromLuminance(void)
{
	size_t numPixels = mWidth * mHeight;
	size_t pointer = 0;
	for (size_t i = 0; i < numPixels; ++i)
	{
		Ogre::ColourValue colour;
		Ogre::PixelUtil::unpackColour(&colour, mFormat, (mBuffer + pointer));
		colour.a = (colour.r + colour.g + colour.b)/3;
		Ogre::PixelUtil::packColour(colour, mFormat, (mBuffer + pointer));
		pointer += Ogre::PixelUtil::getNumElemBytes(mFormat);
	}
}
//-----------------------------------------------------------------------
void ImageX::filterAlphaFromBackgroundColour(const Ogre::ColourValue& backgroundColour)
{
	size_t numPixels = mWidth * mHeight;
	size_t pointer = 0;
	for (size_t i = 0; i < numPixels; ++i)
	{
		Ogre::ColourValue colour;
		Ogre::PixelUtil::unpackColour(&colour, mFormat, (mBuffer + pointer));
		if (colour.r == backgroundColour.r && colour.g == backgroundColour.g && colour.b == backgroundColour.b)
		{
			colour.a = 0.0f;
		}
		Ogre::PixelUtil::packColour(colour, mFormat, (mBuffer + pointer));
		pointer += Ogre::PixelUtil::getNumElemBytes(mFormat);
	}
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Recorder::Recorder(Ogre::Camera* camera) :
	FrameListener(),
	mCamera(camera),
	mFramesPerSecond(27),
	mFrameLength(0.037f),
	mVideoDirectory(Ogre::StringUtil::BLANK),
	mFileNameSuffix(".png"),
	mParticleSystem(0),
	mTimeElapsed(0.0f),
	mTimeElapsedFrame(0.0f),
	mStart(0.0f),
	mEnd(0.0f),
	mImageWidth(640),
	mImageHeight(480),
	mRecording(false),
	mPauseTime(0.2f),
	mRenderTexture(0),
	mFrameCounter(0),
	mImageData(0),
	mPixelBox(),
	mFilter(IF_NONE),
	mBackgroundColour(Ogre::ColourValue::Black),
	mInitialised(false),
	mOldViewport(0)
{
	// Create an image
	mImage = new ImageX();
	mPixelFormat = Ogre::PF_R8G8B8A8;

	// Register itself to root
	Ogre::Root::getSingleton().addFrameListener(this);

	// Store the original viewport
	mOldViewport = camera->getViewport();
}
//-----------------------------------------------------------------------
Recorder::~Recorder(void)
{
	// Delete image
	if (mImage)
	{
		delete mImage;
	}

	// Delete image memory
	if (mImageData)
	{
		OGRE_FREE(mImageData, Ogre::MEMCATEGORY_RENDERSYS);
	}

	// Unregister itself from root
	Ogre::Root::getSingleton().removeFrameListener(this);

	// Delete material
	Ogre::MaterialManager::getSingleton().remove("PURecorderTextureMaterial");

	// Delete texture
	Ogre::TextureManager::getSingleton().remove("PURecorderTexture");
}
//-----------------------------------------------------------------------
void Recorder::setPauseTime (const ParticleUniverse::Real pauseTime)
{
	mPauseTime = pauseTime;
}
//-----------------------------------------------------------------------
void Recorder::setFramesPerSecond (const size_t framesPerSecond)
{
	mFramesPerSecond = framesPerSecond;
	mFrameLength = 1.0f / (ParticleUniverse::Real)framesPerSecond;
}
//-----------------------------------------------------------------------
void Recorder::setFileNameSuffix (const Ogre::String& fileNameSuffix)
{
	if (fileNameSuffix == ".png" ||
		fileNameSuffix == ".jpg" ||
		fileNameSuffix == ".tiff")
	{
		mFileNameSuffix = fileNameSuffix;
	}
	else
	{
		// The suffix in unknown, so choose a png format
		mFileNameSuffix = ".png";
	}
}
//-----------------------------------------------------------------------
void Recorder::setVideoDirectory (const Ogre::String& videoDirectory)
{
	mVideoDirectory = videoDirectory;
}
//-----------------------------------------------------------------------
void Recorder::setImageWidth (const ParticleUniverse::Real imageWidth, bool realloc)
{
	mImageWidth = imageWidth;
	if (realloc)
	{
		_reallocImageMemory();
	}
}
//-----------------------------------------------------------------------
void Recorder::setImageHeight (const ParticleUniverse::Real imageHeight, bool realloc)
{
	mImageHeight = imageHeight;
	if (realloc)
	{
		_reallocImageMemory();
	}
}
//-----------------------------------------------------------------------
void Recorder::setBackgroundColour(const Ogre::ColourValue& backgroundColour)
{
	mBackgroundColour = backgroundColour;
}
//-----------------------------------------------------------------------
void Recorder::setFilter(const ImageFilter& filter)
{
	mFilter = filter;	
}
//-----------------------------------------------------------------------
void Recorder::setParticleSystem (ParticleUniverse::ParticleSystem* particleSystem)
{
	mParticleSystem = particleSystem;
}
//-----------------------------------------------------------------------
void Recorder::record (ParticleUniverse::Real start, ParticleUniverse::Real end)
{
	if (!mInitialised && mCamera)
	{
		// Create render tot texture
		Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().createManual("PURecorderTexture", 
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
			Ogre::TEX_TYPE_2D, 
			mImageWidth,
			mImageHeight,
			0, 
			Ogre::PF_R8G8B8, 
			Ogre::TU_RENDERTARGET);
		mRenderTexture = texture->getBuffer()->getRenderTarget();
		mRenderTexture->addViewport(mCamera);

		// Create material
		Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
		Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(16);
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("PURecorderTextureMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); 
		material->getTechnique(0)->getPass(0)->createTextureUnitState("PURecorderTexture"); 
		material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
		material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
		material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
		mInitialised = true;
	}

	if (!mRecording)
	{
		mRecording = true;
		mTimeElapsed = 0.0f;
		mTimeElapsedFrame = 0.0f;
		mFrameCounter = 0;
		mStart = start;
		mEnd = end;
	}
}
//-----------------------------------------------------------------------
void Recorder::stop (void)
{
	mRecording = false;
	mEnd = 0.0f;

	if (mCamera)
	{
		mCamera->_notifyViewport(mOldViewport); // Camera must use its original viewport
	}
}
//-----------------------------------------------------------------------
bool Recorder::isRecording (void)
{
	return mRecording;
}
//-----------------------------------------------------------------------
bool Recorder::frameEnded (const Ogre::FrameEvent& evt)
{
	if (mRecording)
	{
		if (!mParticleSystem || mParticleSystem->getState() == ParticleUniverse::ParticleSystem::PSS_PAUSED)
		{
			return true;
		}

		mTimeElapsedFrame += evt.timeSinceLastFrame;
		mTimeElapsed += evt.timeSinceLastFrame;
		if (mTimeElapsed > mEnd)
		{
			// Not recording
			mRecording = false;
		}
		else
		{
			if (mTimeElapsed >= mStart)
			{
				if (mTimeElapsedFrame > mFrameLength)
				{
					/** Pause the particle system for a specific time to prevent spikes in case saving to disk takes a long time.
						Do this only for a running particle script!
					*/
					mParticleSystem->pause(mPauseTime);

					// Update and save render texture to disk
					mRenderTexture->update();
					Ogre::String fileName = "Frame_" + 
						Ogre::StringConverter::toString(mFrameCounter) + 
						mFileNameSuffix;
					Ogre::String fullName;
					if (mVideoDirectory != Ogre::StringUtil::BLANK)
					{
						fullName = mVideoDirectory + "/" + fileName;
					}
					else
					{
						fullName = fileName;
					}
					if (mFilter == IF_NONE)
					{
						// No filtering, so do it directly
						mRenderTexture->writeContentsToFile(fullName);
					}
					else
					{
						// Filtering reguired, so introduce the ImageX
						_allocImageMemoryIfNeeded();
						mRenderTexture->copyContentsToMemory(*mPixelBox, Ogre::RenderTarget::FB_AUTO);
						mImage->loadDynamicImage(mImageData, mImageWidth, mImageHeight, 1, mPixelFormat, false, 1, 0);
						switch(mFilter)
						{
							case IF_ALPHA_FROM_LUMINANCE:
								mImage->filterAlphaFromLuminance();
							break;

							case IF_ALPHA_FROM_BACKGROUND_COLOUR:
								mImage->filterAlphaFromBackgroundColour(mBackgroundColour);
							break;
						}
						mImage->save(fullName);
					}
					mFrameCounter++;
					mTimeElapsedFrame =- mFrameLength;
				}
			}
		}
	}

	return true;
}
//-----------------------------------------------------------------------
void Recorder::_reallocImageMemory(void)
{
	if (mImageData)
	{
		OGRE_FREE(mImageData, Ogre::MEMCATEGORY_RENDERSYS);
		mImageData = 0;
	}
	_allocImageMemoryIfNeeded();
}
//-----------------------------------------------------------------------
void Recorder::_allocImageMemoryIfNeeded(void)
{
	if (!mImageData)
	{
		mImageData = OGRE_ALLOC_T(Ogre::uchar, mImageWidth * mImageHeight * Ogre::PixelUtil::getNumElemBytes(mPixelFormat), Ogre::MEMCATEGORY_RENDERSYS);
		mPixelBox = new Ogre::PixelBox(mImageWidth, mImageHeight, 1, mPixelFormat, mImageData);
	}
}
