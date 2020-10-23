/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

 Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PU_RECORDER_H__
#define __PU_RECORDER_H__

#include "ParticleUniverseSystem.h"


/** Extension of the Image class.
*/
class ImageX : public Ogre::Image
{
	public:
		ImageX() : Ogre::Image() {};

		/** Filter functions
		*/
		void filterAlphaFromLuminance();
		void filterAlphaFromBackgroundColour(const Ogre::ColourValue& colour);
};

/** the Recorder is used to record a ParticleSystem in action. Its output is a range of images.
*/
class Recorder : public Ogre::FrameListener
{
	public:

		enum ImageFilter
		{
			IF_NONE,
			IF_ALPHA_FROM_LUMINANCE,
			IF_ALPHA_FROM_BACKGROUND_COLOUR
		};

		Recorder(Ogre::Camera* camera);
		virtual ~Recorder(void);

		/** 
		*/
		void setPauseTime (const ParticleUniverse::Real pauseTime);

		/** 
		*/
		void setFramesPerSecond (const size_t framesPerSecond);
			
		/** 
		*/
		void setFileNameSuffix (const Ogre::String& fileNameSuffix);
			
		/** 
		*/
		void setVideoDirectory (const Ogre::String& videoDirectory);

		/** 
		*/
		void setImageWidth (const ParticleUniverse::Real imageWidth, bool realloc = true);

		/** 
		*/
		void setImageHeight (const ParticleUniverse::Real imageHeight, bool realloc = true);

		/** 
		*/
		void setBackgroundColour(const Ogre::ColourValue& backgroundColour);

		/** 
		*/
		void setFilter(const ImageFilter& filter);

		/** 
		*/
		void setParticleSystem (ParticleUniverse::ParticleSystem* particleSystem);

		/** 
		*/
		void record (ParticleUniverse::Real start = 0.0f, ParticleUniverse::Real end = 3.0f);

		/** 
		*/
		void stop (void);

		/** 
		*/
		bool isRecording (void);

		/** 
		*/
		bool frameEnded(const Ogre::FrameEvent& evt);

	protected:

		/** Reallocate memory for the images
		*/
		void _reallocImageMemory(void);

		/** Allocate memory for the images
		*/
		void _allocImageMemoryIfNeeded(void);

		size_t mFramesPerSecond;
		ParticleUniverse::Real mFrameLength;
		ParticleUniverse::Real mTimeElapsed;
		ParticleUniverse::Real mTimeElapsedFrame;
		ParticleUniverse::Real mStart;
		ParticleUniverse::Real mEnd;
		Ogre::String mVideoDirectory;
		Ogre::String mFileNameSuffix;
		ParticleUniverse::uint mImageWidth;
		ParticleUniverse::uint mImageHeight;
		ParticleUniverse::ParticleSystem* mParticleSystem;
		Ogre::Camera* mCamera;
		bool mRecording;
		ParticleUniverse::Real mPauseTime;
		Ogre::RenderTexture* mRenderTexture;
		size_t mFrameCounter;
		bool mInitialised;
		ImageX* mImage;
		Ogre::uchar* mImageData;
		Ogre::PixelBox* mPixelBox;
		ImageFilter mFilter;
		Ogre::PixelFormat mPixelFormat;
		Ogre::ColourValue mBackgroundColour;
		Ogre::Viewport* mOldViewport;
};

#endif