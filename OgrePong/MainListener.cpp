#include "MainListener.h"
#include "AIManager.h"
#include "InputHandler.h"
#include "World.h"
#include "Camera.h"
#include <ois.h>


MainListener::MainListener(Ogre::RenderWindow *mainWindow, InputHandler *inputManager, AIManager *aIManager, World *world, PongCamera *cam) :
mRenderWindow(mainWindow), mInputHandler(inputManager), mAIManager(aIManager), mWorld(world), mPongCamera(cam)
{
	x = 0;
	countTimer = 0;
}


// On every frame, call the appropriate managers
bool 
	MainListener::frameStarted(const Ogre::FrameEvent &evt)
{
	float time = evt.timeSinceLastFrame;
	if (time > 0.5)
	{
		time = 0.5;
	}
	countTimer += time;

	if (countTimer > 1/120.0f)
	{
		mAIManager->Think(countTimer);
		mInputHandler->Think(countTimer);
		mWorld->Think(countTimer);
		mPongCamera->Think(countTimer);
		countTimer = 0;
	}
	bool keepGoing = true;
	if (mInputHandler->IsKeyDown(OIS::KC_ESCAPE) || mRenderWindow->isClosed())
	{
		keepGoing = false;
	}
	
	return keepGoing;
}