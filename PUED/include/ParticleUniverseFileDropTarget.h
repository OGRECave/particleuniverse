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

#ifndef __PUED_FILE_DROP_TARGET_H__
#define __PUED_FILE_DROP_TARGET_H__

#include "wx/ogre/prerequisites.h"
#include <wx/dnd.h>

/**	Class to handle drag and drop of files
*/
class ParticleUniverseEditorFrame;
class FileDropTarget : public wxFileDropTarget
{
	public:
		FileDropTarget(ParticleUniverseEditorFrame* parent);
		virtual ~FileDropTarget(void){};

		// Overridden from wxFileDropTarget
		virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

		// Load the program, material, skeleton, mesh
		bool validateMesh(Ogre::MeshPtr mesh, const Ogre::String& path);
		bool validateSkeleton(Ogre::SkeletonPtr skeleton, const Ogre::String& path);
		bool validateMaterial(Ogre::MaterialPtr material, 
			const Ogre::String& path, 
			bool& fragmentLocationAdded, 
			bool& vertexLocationAdded, 
			bool& geometryLocationAdded, 
			bool& textureLocationAdded);
		bool validateFragmentShader(Ogre::Pass* pass, const Ogre::String& path);
		bool validateVertexShader(Ogre::Pass* pass, const Ogre::String& path);
		bool validateGeometryShader(Ogre::Pass* pass, const Ogre::String& path);
		bool validateTextures(Ogre::Pass* pass, const Ogre::String& path);
		bool loadSkeletonFile(const Ogre::String& baseNameAndExtension, const Ogre::String& path);
		bool loadMaterialFile(const Ogre::String& baseNameAndExtension, const Ogre::String& path);
		bool loadMaterial(const Ogre::String& materialName, 
			const Ogre::String& path, 
			bool& materialLocationAdded,
			bool& fragmentLocationAdded, 
			bool& vertexLocationAdded, 
			bool& geometryLocationAdded, 
			bool& textureLocationAdded);
		bool loadMeshFile(const Ogre::String& meshName, const Ogre::String& path);
		void addDirectoryToResources(const Ogre::String& path, const Ogre::String& name, const Ogre::String& fileType);
		void initialiseResourceGroup(const Ogre::String& location);
		void displayMessageFileCannotBeLoaded(const wxString& filename);
		void displayMessageNumberOfLoadedFiles(unsigned int loadedFiles);

	protected:
		ParticleUniverseEditorFrame* mFrame;

};

#endif
