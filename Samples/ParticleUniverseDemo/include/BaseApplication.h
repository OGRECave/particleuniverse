#ifndef BASE_APPLICATION_H
#define BASE_APPLICATION_H

#include <OgreWindowEventUtilities.h>
#include <OgreFrameListener.h>

#include <OIS.h>

namespace Ogre
{
    class Root;
    class RenderWindow;
}

class Window; class InputManager;

class BaseApplication : public Ogre::WindowEventListener, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
	BaseApplication();
	~BaseApplication();
	
	void run();
	
protected:
	void windowResized(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw);
			
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt) { mOISKeyboard->capture(); mOISMouse->capture(); return !mShutdown; }

	bool mShutdown;

	Ogre::Root* mRoot;

	OIS::InputManager* mOIS;
	OIS::Mouse* mOISMouse;
	OIS::Keyboard* mOISKeyboard;

	Ogre::RenderWindow* mWindow;

	virtual void createScene() = 0;
	virtual void destroyScene() = 0;

	virtual bool mouseMoved(const OIS::MouseEvent&) {return true;}
	virtual bool mousePressed(const OIS::MouseEvent&, OIS::MouseButtonID) {return true;}
	virtual bool mouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID) {return true;}
	virtual bool keyPressed(const OIS::KeyEvent&) {return true;}
	virtual bool keyReleased(const OIS::KeyEvent&) {return true;}
};

#endif
