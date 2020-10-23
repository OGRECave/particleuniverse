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
#include "ParticleUniverseEditor.h"
#include "ParticleUniverseFileDropTarget.h"
#include <wx/msgdlg.h>

//-----------------------------------------------------------------------
FileDropTarget::FileDropTarget(ParticleUniverseEditorFrame* parent) : 
	wxFileDropTarget(), 
	mFrame(parent)
{
}
//-----------------------------------------------------------------------
bool FileDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	if (filenames.GetCount() == 0)
	{
		// No files dropped
		wxMessageDialog message(mFrame, _("No files are dropped"), _("Drag/drop a file"), wxOK);
		message.ShowModal();
		return true;
	}

	// Handle selected file(s)
	wxString fileName;
	Ogre::String fullyQualifiedFileName;
	Ogre::String baseName;
	Ogre::String baseNameAndExtension;
	Ogre::String extension;
	Ogre::String path;
	wxArrayString::const_iterator it;
	wxArrayString::const_iterator itEnd = filenames.end();
	bool meshAvailable = false;

	// 1. First validate whether the filetype is allowed
	for (it = filenames.begin(); it != itEnd; ++it)
	{
		fileName = *it;
		fullyQualifiedFileName = wx2ogre(fileName);
		Ogre::StringUtil::toLowerCase(fullyQualifiedFileName);
		Ogre::StringUtil::splitFullFilename(fullyQualifiedFileName, baseName, extension, path);
		baseNameAndExtension = baseName + "." + extension;

		//if (extension != "material" && extension != "skeleton" && extension != "mesh")
		if (extension != "material" && extension != "mesh")
		{
			wxString fileOrFiles = wxT("file");
			if (filenames.GetCount() > 1)
			{
				fileOrFiles = wxT("files");
			}
			wxString wxExtension = ogre2wx(extension);
			wxMessageDialog message(mFrame, _("File format *.") + wxExtension + _(" is not supported. No ") + fileOrFiles + _(" loaded."), _("Drag/drop a file"), wxOK);
			message.ShowModal();
			return true;
		}
	}

	// 2. Handle material files
	unsigned int loadedFiles = 0;
	for (it = filenames.begin(); it != itEnd; ++it)
	{
		fileName = *it;
		fullyQualifiedFileName = wx2ogre(fileName);
		Ogre::StringUtil::toLowerCase(fullyQualifiedFileName);
		Ogre::StringUtil::splitFullFilename(fullyQualifiedFileName, baseName, extension, path);
		baseNameAndExtension = baseName + "." + extension;

		if (extension == "material")
		{
			// Load material file
			if (!loadMaterialFile(baseNameAndExtension, path))
			{
				// Display the already loaded files, but stop from there
				displayMessageNumberOfLoadedFiles(loadedFiles);
				return true;
			}
			++loadedFiles;
		}
	}

	// 3. Handle mesh file(s)
	for (it = filenames.begin(); it != itEnd; ++it)
	{
		fileName = *it;
		fullyQualifiedFileName = wx2ogre(fileName);
		Ogre::StringUtil::toLowerCase(fullyQualifiedFileName);
		Ogre::StringUtil::splitFullFilename(fullyQualifiedFileName, baseName, extension, path);
		baseNameAndExtension = baseName + "." + extension;
		if (extension == "mesh")
		{
			// Load the mesh file
			if (loadMeshFile(baseNameAndExtension, path))
			{
				// Mesh successfully loaded
				mFrame->doAddMesh(baseNameAndExtension);
				++loadedFiles;
			}
		}
	}

	// Display the loaded files
	displayMessageNumberOfLoadedFiles(loadedFiles);
	return true;
}
//-----------------------------------------------------------------------
bool FileDropTarget::validateMesh(Ogre::MeshPtr mesh, const Ogre::String& path)
{
	bool resourceAdded = false;

	// --------------- Load skeleton ---------------
	if (mesh->hasSkeleton())
	{
		Ogre::String skeletonName = mesh->getSkeletonName();
		resourceAdded = loadSkeletonFile(skeletonName, path);
	}
	
	// --------------- Load material ---------------
	Ogre::Mesh::SubMeshIterator it = mesh->getSubMeshIterator();
	bool materialLocationAdded = false;
	bool fragmentLocationAdded = false;
	bool vertexLocationAdded = false;
	bool geometryLocationAdded = false;
	bool textureLocationAdded = false;
	while (it.hasMoreElements())
	{
		Ogre::SubMesh* subMesh = it.peekNext();
		if (subMesh)
		{
			Ogre::String materialName = subMesh->getMaterialName();
			resourceAdded = loadMaterial(materialName, 
				path, 
				materialLocationAdded,
				fragmentLocationAdded, 
				vertexLocationAdded, 
				geometryLocationAdded, 
				textureLocationAdded);
		}

		if (!resourceAdded)
		{
			it.moveNext();
		}
		else
		{
			// A resource was added, so start all over; the iterator is screwed up if after clearing/initialising the resource group
			it = mesh->getSubMeshIterator();
			resourceAdded = false;
		}
	}

	return true;
}
//-----------------------------------------------------------------------
bool FileDropTarget::validateSkeleton(Ogre::SkeletonPtr skeleton, const Ogre::String& path)
{
	// TODO
	return true;
}
//-----------------------------------------------------------------------
bool FileDropTarget::validateMaterial(Ogre::MaterialPtr material, 
	const Ogre::String& path, 
	bool& fragmentLocationAdded, 
	bool& vertexLocationAdded, 
	bool& geometryLocationAdded,
	bool& textureLocationAdded)
{
	// Run through the material to check whether vertex shaders, fragment shaders and textures are loaded.
	unsigned short numTechniques = material->getNumTechniques();
	Ogre::Technique* technique = 0;
	Ogre::Pass* pass = 0;
	bool resourceAdded = false;
	for (unsigned short iTechnique = 0; iTechnique < numTechniques; ++iTechnique)
	{
		technique = material->getTechnique(iTechnique);
		if (technique)
		{
			unsigned short numPasses = technique->getNumPasses();
			for (unsigned short iPass = 0; iPass < numPasses; ++iPass)
			{
				pass = technique->getPass(iPass);
				if (pass)
				{
					// --------------- Validate fragment programs ---------------
					if (!fragmentLocationAdded)
					{
						resourceAdded = validateFragmentShader(pass, path);
						fragmentLocationAdded = resourceAdded;
					}

					// --------------- Validate vertex programs ---------------
					if (!vertexLocationAdded)
					{
						resourceAdded = validateVertexShader(pass, path);
						vertexLocationAdded = resourceAdded;
					}

					// --------------- Validate geometry programs ---------------
					if (!geometryLocationAdded)
					{
						resourceAdded = validateGeometryShader(pass, path);
						geometryLocationAdded = resourceAdded;
					}

					// --------------- Validate textures (image files) ---------------
					if (!textureLocationAdded)
					{
						resourceAdded = validateTextures(pass, path);
						textureLocationAdded = resourceAdded;
					}
				}
			}
		}
	}
	return resourceAdded;
}
//-----------------------------------------------------------------------
bool FileDropTarget::validateFragmentShader(Ogre::Pass* pass, const Ogre::String& path)
{
	bool resourceAdded = false;
	if (pass->hasFragmentProgram())
	{
		Ogre::String fragmentProgramName = pass->getFragmentProgramName();
		Ogre::ResourcePtr program;
		if (!Ogre::HighLevelGpuProgramManager::getSingleton().resourceExists(fragmentProgramName))
		{
			// Fragment shader does not exist; add it to the resources
			addDirectoryToResources(path, fragmentProgramName, "Fragment shader");
			resourceAdded = true;
			program = Ogre::TextureManager::getSingletonPtr()->load(fragmentProgramName, PU_EDITOR_RESOURCE_LOCATION);
			if (!Ogre::HighLevelGpuProgramManager::getSingleton().resourceExists(fragmentProgramName))
			{
				// Fragment shader still does not exist
				displayMessageFileCannotBeLoaded(ogre2wx(fragmentProgramName));
				throw Ogre::Exception(1, "Fragment shader does not exist", "validateFragmentShader");
			}
		}
		else
		{
			program = Ogre::HighLevelGpuProgramManager::getSingletonPtr()->getByName(fragmentProgramName);
			Ogre::GpuProgramPtr gpuProgram = pass->getFragmentProgram();
			if (program.isNull() || gpuProgram.isNull())
			{
				// Fragment shader still does not exist
				displayMessageFileCannotBeLoaded(ogre2wx(fragmentProgramName));
				throw Ogre::Exception(2, "Fragment shader does not exist", "validateFragmentShader");
			}
			else if (gpuProgram->getSource() == Ogre::StringUtil::BLANK)
			{
				// TODO: Source is never filled, so the directory dialog is always displayed

				// Fragment shader source does not exist
				addDirectoryToResources(path, fragmentProgramName, "Fragment shader");
				resourceAdded = true;
			}
		}
		program->load();
	}
	return resourceAdded;
}
//-----------------------------------------------------------------------
bool FileDropTarget::validateVertexShader(Ogre::Pass* pass, const Ogre::String& path)
{
	bool resourceAdded = false;
	if (pass->hasVertexProgram())
	{
		Ogre::String vertexProgramName = pass->getVertexProgramName();
		Ogre::ResourcePtr program;
		if (!Ogre::HighLevelGpuProgramManager::getSingleton().resourceExists(vertexProgramName))
		{
			// Vertex shader does not exist; add it to the resources
			addDirectoryToResources(path, vertexProgramName, "Vertex shader");
			resourceAdded = true;
			program = Ogre::TextureManager::getSingletonPtr()->load(vertexProgramName, PU_EDITOR_RESOURCE_LOCATION);
			if (!Ogre::HighLevelGpuProgramManager::getSingleton().resourceExists(vertexProgramName))
			{
				// Vertex shader still does not exist
				displayMessageFileCannotBeLoaded(ogre2wx(vertexProgramName));
				throw Ogre::Exception(1, "Vertex shader does not exist", "validateVertexShader");
			}
		}
		else
		{
			program = Ogre::HighLevelGpuProgramManager::getSingletonPtr()->getByName(vertexProgramName);
			Ogre::GpuProgramPtr gpuProgram = pass->getVertexProgram();
			if (program.isNull() || gpuProgram.isNull())
			{
				// Vertex shader still does not exist
				displayMessageFileCannotBeLoaded(ogre2wx(vertexProgramName));
				throw Ogre::Exception(2, "Vertex shader does not exist", "validateVertexShader");
			}
			else if (gpuProgram->getSource() == Ogre::StringUtil::BLANK)
			{
				// TODO: Source is never filled, so the directory dialog is always displayed

				// Vertex shader source does not exist
				addDirectoryToResources(path, vertexProgramName, "Vertex shader");
				resourceAdded = true;
			}
		}
		program->load();
	}
	return resourceAdded;
}
//-----------------------------------------------------------------------
bool FileDropTarget::validateGeometryShader(Ogre::Pass* pass, const Ogre::String& path)
{
	bool resourceAdded = false;
	if (pass->hasGeometryProgram())
	{
		Ogre::String geometryProgramName = pass->getGeometryProgramName();
		Ogre::ResourcePtr program;
		if (!Ogre::HighLevelGpuProgramManager::getSingleton().resourceExists(geometryProgramName))
		{
			// Geometry shader does not exist; add it to the resources
			addDirectoryToResources(path, geometryProgramName, "Geometry shader");
			resourceAdded = true;
			program = Ogre::TextureManager::getSingletonPtr()->load(geometryProgramName, PU_EDITOR_RESOURCE_LOCATION);
			if (!Ogre::HighLevelGpuProgramManager::getSingleton().resourceExists(geometryProgramName))
			{
				// Geometry shader still does not exist
				displayMessageFileCannotBeLoaded(ogre2wx(geometryProgramName));
				throw Ogre::Exception(1, "Geometry shader does not exist", "validateGeometryShader");
			}
		}
		else
		{
			program = Ogre::HighLevelGpuProgramManager::getSingletonPtr()->getByName(geometryProgramName);
			Ogre::GpuProgramPtr gpuProgram = pass->getGeometryProgram();
			if (program.isNull() || gpuProgram.isNull())
			{
				// Geometry shader still does not exist
				displayMessageFileCannotBeLoaded(ogre2wx(geometryProgramName));
				throw Ogre::Exception(2, "Geometry shader does not exist", "validateGeometryShader");
			}
			else if (gpuProgram->getSource() == Ogre::StringUtil::BLANK)
			{
				// TODO: Source is never filled, so the directory dialog is always displayed

				// Geometry shader source does not exist
				addDirectoryToResources(path, geometryProgramName, "Geometry shader");
				resourceAdded = true;
			}
		}
		program->load();
	}
	return resourceAdded;
}
//-----------------------------------------------------------------------
bool FileDropTarget::validateTextures(Ogre::Pass* pass, const Ogre::String& path)
{
	bool resourceAdded = false;
	unsigned short numTexures = pass->getNumTextureUnitStates();
	for (unsigned short iTUT = 0; iTUT < numTexures; ++iTUT)
	{
		Ogre::TextureUnitState* textureUnit = pass->getTextureUnitState(iTUT);
		if (textureUnit)
		{
			unsigned int numFrames = textureUnit->getNumFrames();
			if (numFrames > 0)
			{
				Ogre::String frameTextureName = textureUnit->getFrameTextureName(0);
				Ogre::ResourcePtr texture;
				if (!Ogre::TextureManager::getSingleton().resourceExists(frameTextureName))
				{
					// Texture does not exist; add it to the resources
					addDirectoryToResources(path, frameTextureName, "Texture");
					resourceAdded = true;
					texture = Ogre::TextureManager::getSingletonPtr()->load(frameTextureName, PU_EDITOR_RESOURCE_LOCATION);
					if (!Ogre::TextureManager::getSingleton().resourceExists(frameTextureName))
					{
						// Texture still does not exist
						displayMessageFileCannotBeLoaded(ogre2wx(frameTextureName));
						throw Ogre::Exception(1, "Texture does not exist", "validateTextures");
					}
				}
				else
				{
					texture = Ogre::TextureManager::getSingletonPtr()->getByName(frameTextureName);
					if (texture.isNull())
					{
						// Texture still does not exist
						displayMessageFileCannotBeLoaded(ogre2wx(frameTextureName));
						throw Ogre::Exception(2, "Texture does not exist", "validateTextures");
					}
					else if (!texture->isLoaded())
					{
						// Texture does exist, but is not loaded
						addDirectoryToResources(path, frameTextureName, "Texture");
						resourceAdded = true;
					}
				}
				texture->load();
			}
		}
	}
	return resourceAdded;
}
//-----------------------------------------------------------------------
bool FileDropTarget::loadSkeletonFile(const Ogre::String& baseNameAndExtension, const Ogre::String& path)
{
	bool resourceAdded = false;

	// Load the skeleton file
	wxString wxBaseNameAndExtension = ogre2wx(baseNameAndExtension);
	Ogre::SkeletonPtr skeleton;
	if (!Ogre::SkeletonManager::getSingleton().resourceExists(baseNameAndExtension))
	{
		// Skeleton does not exist
		addDirectoryToResources(path, baseNameAndExtension, "Skeleton");
		resourceAdded = true;
	}

	try
	{
		skeleton = Ogre::SkeletonManager::getSingleton().getByName(baseNameAndExtension, PU_EDITOR_RESOURCE_LOCATION);
		if (skeleton.isNull())
		{
			// Skeleton not loaded. It stops here.
			displayMessageFileCannotBeLoaded(wxBaseNameAndExtension);
			return false;
		}
		else if (!skeleton->isLoaded())
		{
			// Skeleton does exist, but is not loaded?
			addDirectoryToResources(path, baseNameAndExtension, "Skeleton");
			resourceAdded = true;
		}
	}

	catch (Ogre::Exception e)
	{
		// Skeleton not loaded
		displayMessageFileCannotBeLoaded(wxBaseNameAndExtension);
		return false;
	}

	skeleton->load();
	return resourceAdded;
}
//-----------------------------------------------------------------------
bool FileDropTarget::loadMaterialFile(const Ogre::String& baseNameAndExtension, const Ogre::String& path)
{
	// Load the material file
	wxString wxBaseNameAndExtension = ogre2wx(baseNameAndExtension);
	Ogre::MaterialPtr material;
	bool resourceAdded = false;

	// Assume that programs and textures referenced to in 1 material file are in the same location
	bool fragmentLocationAdded = false;
	bool vertexLocationAdded = false;
	bool geometryLocationAdded = false;
	bool textureLocationAdded = false;

	Ogre::String materialFileName;
	try
	{
		initialiseResourceGroup(path);
		Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(baseNameAndExtension, PU_EDITOR_RESOURCE_LOCATION);
		if (stream.isNull())
		{
			// Cannot open the file
			displayMessageFileCannotBeLoaded(wxBaseNameAndExtension);
			return false;
		}
		else
		{
			// Parse the material
			Ogre::MaterialManager::getSingleton().parseScript(stream, PU_EDITOR_RESOURCE_LOCATION);
			stream->close();

			// Search the materials from the file and compile them
			Ogre::ResourceManager::ResourceMapIterator materialIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();
			while (materialIterator.hasMoreElements())
			{
				Ogre::MaterialPtr material = materialIterator.peekNextValue();
				if (!material.isNull())
				{
					materialFileName = material->getOrigin();
					Ogre::StringUtil::toLowerCase(materialFileName);
					if (materialFileName == wx2ogre(wxBaseNameAndExtension))
					{
						// This was a material from the file that was just parsed
						material->load();

						// Validate the material
						if (!(fragmentLocationAdded && vertexLocationAdded && geometryLocationAdded && geometryLocationAdded && textureLocationAdded))
						{
							resourceAdded = validateMaterial(material, 
								path, 
								fragmentLocationAdded, 
								vertexLocationAdded, 
								geometryLocationAdded, 
								textureLocationAdded);
						}

						// Compile again in case the program and textures were loaded
						material->compile();
					}
				}
				if (!resourceAdded)
				{
					materialIterator.moveNext();
				}
				else
				{
					// A resource was added, so start all over; the iterator is screwed up if after clearing/initialising the resource group
					materialIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();
					resourceAdded = false;
				}
			}
		}
	}

	catch (Ogre::Exception e)
	{
		// Material file not loaded
		displayMessageFileCannotBeLoaded(wxBaseNameAndExtension);
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------
bool FileDropTarget::loadMaterial(const Ogre::String& materialName, 
	const Ogre::String& path,
	bool& materialLocationAdded, 
	bool& fragmentLocationAdded, 
	bool& vertexLocationAdded, 
	bool& geometryLocationAdded, 
	bool& textureLocationAdded)
{
	// Check whether the material (not the file) has been loaded. If not, load the material
	bool resourceAdded = false;
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(materialName, PU_EDITOR_RESOURCE_LOCATION);
	if (material.isNull())
	{
		// Material does not exist
		if (!materialLocationAdded)
		{
			addDirectoryToResources(path, materialName, "Material");
			materialLocationAdded = true;
			resourceAdded = true;
		}
		material = Ogre::MaterialManager::getSingleton().getByName(materialName, PU_EDITOR_RESOURCE_LOCATION);
		if (material.isNull())
		{
			// Material not loaded. It stops here, what is there more to do?
			displayMessageFileCannotBeLoaded(ogre2wx(materialName));
			return false;
		}
	}
	material->load();

	// Load and compile the material. If other resources are missing they are loaded also
	bool added = validateMaterial(material, 
		path, 
		fragmentLocationAdded, 
		vertexLocationAdded, 
		geometryLocationAdded, 
		textureLocationAdded);
	resourceAdded = resourceAdded || added;

	// Compile again in case the program and textures were loaded
	material->compile();

	return resourceAdded;
}
//-----------------------------------------------------------------------
bool FileDropTarget::loadMeshFile(const Ogre::String& baseNameAndExtension, const Ogre::String& path)
{
	// Load the mesh file
	wxString wxBaseNameAndExtension = ogre2wx(baseNameAndExtension);
	Ogre::MeshPtr mesh;
	try
	{
		initialiseResourceGroup(path);
		mesh = Ogre::MeshManager::getSingletonPtr()->load(baseNameAndExtension, PU_EDITOR_RESOURCE_LOCATION);

		// Validate the mesh (check for skeleton file and material file)
		validateMesh(mesh, path);
	}

	catch (Ogre::Exception e)
	{
		// Mesh not loaded
		displayMessageFileCannotBeLoaded(wxBaseNameAndExtension);
		return false;
	}

	if (mesh.isNull())
	{
		// Mesh not loaded
		displayMessageFileCannotBeLoaded(wxBaseNameAndExtension);
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------
void FileDropTarget::addDirectoryToResources(const Ogre::String& path, const Ogre::String& name, const Ogre::String& fileType)
{
	wxString title = ogre2wx(name) + wxT(" (") + ogre2wx(fileType) + _(") cannot be found. Select a directory where it can be loaded");
	wxString& dir = wxDirSelector(title, ogre2wx(path));
	if (!dir.empty())
	{
		initialiseResourceGroup(wx2ogre(dir));
	}
}
//-----------------------------------------------------------------------
void FileDropTarget::initialiseResourceGroup(const Ogre::String& location)
{
	if (!Ogre::ResourceGroupManager::getSingletonPtr()->resourceLocationExists(location, PU_EDITOR_RESOURCE_LOCATION))
	{
		Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(location, "FileSystem", PU_EDITOR_RESOURCE_LOCATION);
		Ogre::ResourceGroupManager::getSingletonPtr()->clearResourceGroup(PU_EDITOR_RESOURCE_LOCATION);
		Ogre::ResourceGroupManager::getSingletonPtr()->initialiseResourceGroup(PU_EDITOR_RESOURCE_LOCATION);
	}
}
//-----------------------------------------------------------------------
void FileDropTarget::displayMessageFileCannotBeLoaded(const wxString& filename)
{
	// Cannot open the file
	wxMessageDialog message(mFrame, filename + _(" was not succesfully loaded"), _("Drag/drop a file"), wxOK);
	message.ShowModal();
}
//-----------------------------------------------------------------------
void FileDropTarget::displayMessageNumberOfLoadedFiles(unsigned int loadedFiles)
{
	Ogre::String numberOfLoadedFiles = Ogre::StringConverter::toString(loadedFiles);
	wxMessageDialog message(mFrame, ogre2wx(numberOfLoadedFiles) + _(" files succesfully loaded"), _("Drag/drop a file"), wxOK);
	message.ShowModal();
}
