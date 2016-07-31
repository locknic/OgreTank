#include "OgreCamera.h"
#include "Camera.h"
#include "World.h"
#include "OgreVector3.h"
#include "OgreSceneNode.h"

PongCamera::PongCamera(Ogre::Camera *renderCamera,Ogre::Camera *renderCamera2, World *world) :
mRenderCamera(renderCamera), mRenderCamera2(renderCamera2), mWorld(world)
{
	mRenderCamera->setNearClipDistance(2);
	mRenderCamera2->setNearClipDistance(2);
}

void
PongCamera::Think(float time)
{
	mRenderCamera->setPosition(mWorld->getPlayerNode()->getPosition());
	mRenderCamera->setOrientation(mWorld->getPlayerNode()->getOrientation());
	//mRenderCamera->lookAt(mWorld->getPlayerNode()->getPosition());
	mRenderCamera->yaw(Ogre::Degree(-90));
	mRenderCamera->moveRelative(Ogre::Vector3(0, 10, 20));

	mRenderCamera2->setPosition(mWorld->getComputerNode()->getPosition());
	mRenderCamera2->setOrientation(mWorld->getComputerNode()->getOrientation());
	//mRenderCamera->lookAt(mWorld->getPlayerNode()->getPosition());
	mRenderCamera2->yaw(Ogre::Degree(-90));
	mRenderCamera2->moveRelative(Ogre::Vector3(0, 10, 20));
}
