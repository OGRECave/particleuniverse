/*
 * Copyright (C) 2007-2008 Martin Pieuchot 
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#ifndef _WX_OGRE_CONTROL_H_
#define _WX_OGRE_CONTROL_H_

#include <Ogre.h>
#include <wx/wx.h>
#include "ParticleUniverseEditorListener.h"

/** Manages an Ogre rendering window and its viewport. 
 *
 * @remark This class creates an embedded Ogre::RenderWindow, set up 
 *         and manages a Ogre::Viewport for a given Ogre::Camera and
 *         provides basic input handling to control the camera
 *         movements.
 *
 * @author Martin Pieuchot
 */ 
class ParticleUniverseEditorFrame;
class wxOgreControl : public wxControl
{
    DECLARE_DYNAMIC_CLASS(wxOgreControl)
    DECLARE_EVENT_TABLE()

public:

    /** Default constructor. */
    wxOgreControl(); 

    /** Constructs a window, with an embedded Ogre rendering window. */    
    wxOgreControl(wxWindow* parent, wxWindowID id = wxID_ANY,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        long style = wxSUNKEN_BORDER | wxTRANSPARENT_WINDOW,
                        const wxValidator& val = wxDefaultValidator,
                        const wxString& name = wxT("OgreControl"));

    /** Destructor, call Destroy to clean Ogre stuff.
     *
     * @remark This destructor is needed to correctly destroy ogre
     *         instances when the control is not deleted using the 
     *         Destroy method, for example when destroyed by a
     *         wxNotebook::DeletePage call.
     *
     * @see Destroy
     */
    virtual ~wxOgreControl();

    /** Constructs a window, with an embedded Ogre rendering window. */    
    virtual bool Create(wxWindow* parent, wxWindowID id = wxID_ANY,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        long style = wxSUNKEN_BORDER | wxTRANSPARENT_WINDOW,
                        const wxValidator& val = wxDefaultValidator,
                        const wxString& name = wxT("OgreControl"));

    /** Destructor, deletes the camera if exists, */ 
    virtual bool Destroy();

    /** Render a frame and update the rendering window. */  
    virtual void Update();

    /** Returns a small 3:4 window size.
     *
     * @return A size of 320x240.
     */
    virtual wxSize DoGetBestSize() const { return wxSize(320, 240); }


    /** Creates a scene manager instance of a given type.
     * 
     * @see Ogre::createSceneManager
     */ 
    virtual Ogre::SceneManager* CreateSceneManager(const Ogre::String& tn,
                                                   const Ogre::String& name="");

    /** Creates a SceneManager instance of a given type.
     * 
     * @see Ogre::createSceneManager
     */ 
    virtual Ogre::SceneManager* CreateSceneManager(Ogre::SceneTypeMask tm,
                                                   const Ogre::String& name="");


    /** Set-up the rendering window for the given scene manager.
     *
     * This method creates a camera and attaches a viewport to the 
     * rendering window for the given scene manager.
     *
     * @see SetCamera
     * @see AddViewport 
     */
    virtual void SetSceneManager(Ogre::SceneManager* sm);

    /** Returns the scene manager for this rendering window.
     */
	virtual Ogre::SceneManager* GetSceneManager(void);

    /** Creates a viewport and attach the given camera to it.
     *
     * @remark If an old camera was used it is destroyed. The aspect
     *         ration of the camera is automatically updated.
     *
     * @see AddViewport
     */
    virtual void SetCamera(Ogre::Camera* cam);

    /** Rotates the camera anticlockwise, around its local x, y and
     *  z axis.
     *
     * @see Ogre::Camera::roll
     * @see Ogre::Camera::yaw
     * @see Ogre::Camera::pitch
     */
    virtual void RotateCamera(float relX, float relY, float relZ);

    /** Moves the camera's position by the x, y, and z offsets 
     * along it's own axes (relative to orientation).
     *
     * @see Ogre::Camera::moveRelative
     */
    virtual void TranslateCamera(float x, float y, float z);

    /** Sets the level of rendering detail required from this camera. 
     *
     * @see Ogre::Camera::setPolygonMode
     */
    virtual void SetPolygonMode(const Ogre::PolygonMode& pm);

    /** Virtual method called by the default mouse handler.
     *
     * @param pt the position clicked by the user.
     *
     * @remark This method is defined in order to simplify the 
     *         selection of 3D components. Just define here, in your
     *         custom subcontrol, the action associated with the left
     *         click mouse button. 
     *
     * @see OnMouseMove
     * @see Tutorial 3
     */
    virtual void ProcessSelection(const wxPoint& pt);

    /** Retrieves the control's camera. */
    Ogre::Camera* GetCamera() const { return m_cam; }

    /** Retrieves the control's render window. */
    Ogre::RenderWindow* GetRenderWindow() const { return m_rwin; } 

    /** Retrieves the control's viewport. */
    Ogre::Viewport* GetViewport() const { return m_vp; }

    /** Adds a viewport to the rendering target. 
     *
     * @see Ogre::RenderTarget::addViewport
     */
    void AddViewport(Ogre::Camera* cam, int ZOrder=0, float left=0.0f,
                     float top=0.0f, float  width=1.0f, float height=1.0f);

    /** Detaches the control's render window if exists.
     *
     * @see Ogre::Root:detachRenderTarget
     */
    void DestroyRenderWindow();

    /** Used for mouse movement.
     */
	void setCallbackFrame(ParticleUniverseEditorFrame* frame);


protected:

    virtual void OnSize(wxSizeEvent& event);
    virtual void OnPaint(wxPaintEvent& event);
    virtual void OnMouseMove(wxMouseEvent& event);
    virtual void OnEraseBackground(wxEraseEvent& event);
	void OnIdle(wxIdleEvent& WXUNUSED(event));

    Ogre::SceneManager* m_sm;
    Ogre::Camera*       m_cam;
	ParticleUniverseEditorFrame* mCallbackFrame;

private:

    /** Initializes private variables. */
    virtual void Init();

    /** Creates an Ogre's rendering window.
     *
     * @param name the internal name of the Ogre::RenderWindow
     */
    Ogre::RenderWindow* CreateRenderWindow(const Ogre::String& name);

    /** Retrieves the unique window identifier of this control.
     *
     * @remark This is *the* platform independent method of the
     *         library.
     *
     * @param pl an Ogre pair list passed to the createRenderWinow
     *        function.
     *
     * @see Ogre::Root::createRenderWindow
     */
    void GetParentWindowHandle(Ogre::NameValuePairList& pl);


    Ogre::Root*         m_root;
    Ogre::RenderWindow* m_rwin;
    Ogre::Viewport*     m_vp;


    /** Last x position of the cursor. */
    long m_x;
    /** Last y position of the cursor. */
    long m_y;

    /** Number of rendering windows instantiated. */
    static unsigned int m_instances;
};

#endif
