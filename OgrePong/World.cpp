// My header file.  This should go first!
#include "World.h"

// Ogre header files
#include "Ogre.h"
#include "OgreMath.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreOverlayManager.h"
#include "OgreOverlay.h"
#include "OgreFontManager.h"
#include "OgreTextAreaOverlayElement.h"
#include <OgreEntity.h>
#include "OgreLight.h"
#include "OgreVector3.h"
#include <vector>

// IOS (Input system) header files

#include <ois/ois.h>
#include <string>

// Other input files for my project
#include "Camera.h"
#include "InputHandler.h"



World::World(Ogre::SceneManager *sceneManager, InputHandler *input)   : mSceneManager(sceneManager), mInputHandler(input)
{
	// Global illumination for now.  Adding individual light sources will make you scene look more realistic
	mSceneManager->setAmbientLight(Ogre::ColourValue(0.2,0.2,0.2));
	Ogre::Light* sun = mSceneManager->createLight("sun");
	sun->setDiffuseColour(1,1,1);
	sun->setSpecularColour(1,1,1);
	sun->setType(Ogre::Light::LT_DIRECTIONAL);
	sun->setPosition(Ogre::Vector3(200, 200, 200));
	sun->setDirection(-1, -1, -1);
	// First, we create a scene node, and attach a model to it.  coin.mesh is defined in the Content directory,
	//  under models.  Note that you should probably place all of this creation code into a separate method (or methods)
	//  for your final game

	mCollisionList = new std::vector<Ogre::Entity*>;
	mUpperBoundsEntity = SceneManager()->createEntity("bounds.mesh");
	Ogre::SceneNode *mUpperWallNode = SceneManager()->getRootSceneNode()->createChildSceneNode();
	mUpperWallNode->attachObject(mUpperBoundsEntity);
	mUpperWallNode->scale(1,10,200);
	mUpperWallNode->setPosition(200,10,0);
	mCollisionList->push_back(mUpperBoundsEntity);

	mLowerBoundsEntity = SceneManager()->createEntity("bounds.mesh");
	Ogre::SceneNode *mLowerWallNode = SceneManager()->getRootSceneNode()->createChildSceneNode();
	mLowerWallNode->attachObject(mLowerBoundsEntity);
	mLowerWallNode->scale(1,10,200);
	mLowerWallNode->setPosition(-200,10,0);
	mCollisionList->push_back(mLowerBoundsEntity);

	mLeftBoundsEntity = SceneManager()->createEntity("bounds.mesh");
	Ogre::SceneNode *mLeftWallNode = SceneManager()->getRootSceneNode()->createChildSceneNode();
	mLeftWallNode->attachObject(mLeftBoundsEntity);
	mLeftWallNode->scale(35,10,1);
	mLeftWallNode->setPosition(0,10,200);
	mCollisionList->push_back(mLeftBoundsEntity);

	mRightBoundsEntity = SceneManager()->createEntity("bounds.mesh");
	Ogre::SceneNode *mRightWallNode = SceneManager()->getRootSceneNode()->createChildSceneNode();
	mRightWallNode->attachObject(mRightBoundsEntity);
	mRightWallNode->scale(35,10,1);
	mRightWallNode->setPosition(0,10,-200);
	mCollisionList->push_back(mRightBoundsEntity);

	Ogre::Entity *randomWall = SceneManager()->createEntity("ball.mesh");
	Ogre::SceneNode *mRandomWallNode = SceneManager()->getRootSceneNode()->createChildSceneNode();
	mRandomWallNode->attachObject(randomWall);
	mRandomWallNode->scale(20,20,20);
	mRandomWallNode->setPosition(20,10,20);
	mCollisionList->push_back(randomWall);

	mPlayerEntity =SceneManager()->createEntity("paddle-blue.mesh");
	mPlayerNode = SceneManager()->getRootSceneNode()->createChildSceneNode();
	mPlayerNode->attachObject(mPlayerEntity);
	mPlayerNode->scale(1,1,1);
	mPlayerNode->setPosition(-100,5,-100);
	mCollisionList->push_back(mPlayerEntity);

	mComputerEntity = SceneManager()->createEntity("paddle-orange.mesh");
	mComputerNode = SceneManager()->getRootSceneNode()->createChildSceneNode();
	mComputerNode->attachObject(mComputerEntity);
	mComputerNode->scale(1,1,1);
	mComputerNode->setPosition(100,5,100);
	mCollisionList->push_back(mComputerEntity);

	mBulletEntity = SceneManager()->createEntity("ball.mesh");
	mBulletNode = SceneManager()->getRootSceneNode()->createChildSceneNode();
	mBulletNode->attachObject(mBulletEntity);
	mBulletNode->setPosition(0,-10,0);
	aliveBullet = false;

	mBulletEntity2 = SceneManager()->createEntity("ball.mesh");
	mBulletNode2 = SceneManager()->getRootSceneNode()->createChildSceneNode();
	mBulletNode2->attachObject(mBulletEntity2);
	mBulletNode2->setPosition(0,-10,0);
	aliveBullet2 = false;

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane(
 "ground",
  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
  plane, 
  1500, 1500, 20, 20, 
  true, 
  1, 5, 5, 
  Ogre::Vector3::UNIT_Z);
	Ogre::Entity* groundEntity = mSceneManager->createEntity("ground");
	mSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setCastShadows(false);
	groundEntity->setMaterialName("floor");
	// Yeah, this should be done automatically for all fonts referenced in an overlay file.
	//  But there is a bug in the OGRE code so we need to do it manually.
	Ogre::ResourceManager::ResourceMapIterator iter = Ogre::FontManager::getSingleton().getResourceIterator();
	while (iter.hasMoreElements()) 
	{ 
		iter.getNext()->load(); 
	}

	// Now we will show the sample overlay.  Look in the file Content/Overlays/Example to
	// see how this overlay is defined
	Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();
	
	Ogre::Overlay *playerScoreOverlay = om.getByName("PlayerScore");
	playerScoreOverlay->show();

	Ogre::Overlay *computerScoreOverlay = om.getByName("ComputerScore");
	computerScoreOverlay->show();

	playerScore = 0;
	computerScore = 0;
	mSpeed = 40;
	reloadTimer = 0;
	reloadTimer2 = 0;
}


void 
World::Think(float time)
{
	const float PADDLE_SPEED = mSpeed;
	int direction = 0;

	if (mInputHandler->IsKeyDown(OIS::KC_UP))
	{
		mPlayerNode->translate(time * PADDLE_SPEED, 0, 0, Ogre::SceneNode::TS_LOCAL);
		direction = -1;
	}
	else if (mInputHandler->IsKeyDown(OIS::KC_DOWN))
	{
		mPlayerNode->translate(-time * PADDLE_SPEED, 0, 0, Ogre::SceneNode::TS_LOCAL);
		direction = -1;
	}

	if (mInputHandler->IsKeyDown(OIS::KC_LEFT))
	{
		mPlayerNode->yaw(Ogre::Degree(time * 45));
	}
	else if (mInputHandler->IsKeyDown(OIS::KC_RIGHT))
	{
		mPlayerNode->yaw(Ogre::Degree(-time * 45));
	}

	if (mCollisionList->begin() != mCollisionList->end())
	{
		for(std::vector<Ogre::Entity*>::iterator it = mCollisionList->begin(); it != mCollisionList->end(); ++it) {
			if (mPlayerEntity != (*it) && mPlayerEntity->getWorldBoundingBox().intersects((*it)->getWorldBoundingBox()))
			{
				mPlayerNode->translate(direction * time * PADDLE_SPEED, 0, 0, Ogre::SceneNode::TS_LOCAL);
			}
		}
	}
	
	if(aliveBullet)
	{
		mBulletNode->translate(time * PADDLE_SPEED * 5, 0, 0, Ogre::SceneNode::TS_LOCAL);
		if (mCollisionList->begin() != mCollisionList->end())
		{
			if (mBulletNode->getPosition().y < 0)
			{
				mBulletNode->setPosition(0,-10,0);
				aliveBullet = false;
			}
			else
			{
				for(std::vector<Ogre::Entity*>::iterator it = mCollisionList->begin(); it != mCollisionList->end(); ++it) 
				{
					if (mPlayerEntity != (*it) && mBulletEntity->getWorldBoundingBox().intersects((*it)->getWorldBoundingBox()))
					{
						if ((*it) == mComputerEntity)
						{
							score(true);
						}
						mBulletNode->setPosition(0,-10,0);
						aliveBullet = false;
					}
				}
			}
			mBulletNode->translate(0, -10 * time, 0, Ogre::SceneNode::TS_LOCAL);
		}
	}
	else if (mInputHandler->IsKeyDown(OIS::KC_RETURN) && reloadTimer > 2)
	{
		aliveBullet = true;
		reloadTimer = 0;
		mBulletNode->setPosition(mPlayerNode->getPosition());
		mBulletNode->setOrientation(mPlayerNode->getOrientation());
		mBulletNode->translate(5, 4, 0, Ogre::SceneNode::TS_LOCAL);
	}
	reloadTimer += time;

	int direction2 = 0;

	if (mInputHandler->IsKeyDown(OIS::KC_W))
	{
		mComputerNode->translate(time * PADDLE_SPEED, 0, 0, Ogre::SceneNode::TS_LOCAL);
		direction = -1;
	}
	else if (mInputHandler->IsKeyDown(OIS::KC_S))
	{
		mComputerNode->translate(-time * PADDLE_SPEED, 0, 0, Ogre::SceneNode::TS_LOCAL);
		direction = -1;
	}

	if (mInputHandler->IsKeyDown(OIS::KC_A))
	{
		mComputerNode->yaw(Ogre::Degree(time * 45));
	}
	else if (mInputHandler->IsKeyDown(OIS::KC_D))
	{
		mComputerNode->yaw(Ogre::Degree(-time * 45));
	}

	if (mCollisionList->begin() != mCollisionList->end())
	{
		for(std::vector<Ogre::Entity*>::iterator it = mCollisionList->begin(); it != mCollisionList->end(); ++it) {
			if (mComputerEntity != (*it) && mComputerEntity->getWorldBoundingBox().intersects((*it)->getWorldBoundingBox()))
			{
				mComputerNode->translate(direction * time * PADDLE_SPEED, 0, 0, Ogre::SceneNode::TS_LOCAL);
			}
		}
	}
	
	if(aliveBullet2)
	{
		mBulletNode2->translate(time * PADDLE_SPEED * 5, 0, 0, Ogre::SceneNode::TS_LOCAL);
		if (mCollisionList->begin() != mCollisionList->end())
		{
			if (mBulletNode2->getPosition().y < 0)
			{
				mBulletNode2->setPosition(0,-10,0);
				aliveBullet2 = false;
			}
			else
			{
				for(std::vector<Ogre::Entity*>::iterator it = mCollisionList->begin(); it != mCollisionList->end(); ++it) 
				{
					if (mComputerEntity != (*it) && mBulletEntity2->getWorldBoundingBox().intersects((*it)->getWorldBoundingBox()))
					{
						if ((*it) == mPlayerEntity)
						{
							score(false);
						}
						mBulletNode2->setPosition(0,-10,0);
						aliveBullet2 = false;
					}
				}
			}
			mBulletNode2->translate(0, -10 * time, 0, Ogre::SceneNode::TS_LOCAL);
		}
	}
	else if (mInputHandler->IsKeyDown(OIS::KC_SPACE) && reloadTimer2 > 2)
	{
		aliveBullet2 = true;
		reloadTimer2 = 0;
		mBulletNode2->setPosition(mComputerNode->getPosition());
		mBulletNode2->setOrientation(mComputerNode->getOrientation());
		mBulletNode2->translate(5, 4, 0, Ogre::SceneNode::TS_LOCAL);
	}
	reloadTimer2 += time;

	 //Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();
	 //Ogre::TextAreaOverlayElement *te = (Ogre::TextAreaOverlayElement *) om.getOverlayElement("Sample/Panel/Text1");
	 //te->setCaption("New Caption!");
}

void World::score(bool playerScored)
{
	Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();

	if (playerScored)
	{
		playerScore++;
		Ogre::TextAreaOverlayElement *te = (Ogre::TextAreaOverlayElement *) om.getOverlayElement("PlayerScore/Panel/Text1");
		te->setCaption(std::to_string(playerScore));
		mComputerNode->setPosition(100,5,100);
		mPlayerNode->setPosition(-100,5,-100);
	}
	else
	{
		computerScore++;
		Ogre::TextAreaOverlayElement *te2 = (Ogre::TextAreaOverlayElement *) om.getOverlayElement("ComputerScore/Panel/Text1");
		te2->setCaption(std::to_string(computerScore));
		mComputerNode->setPosition(100,5,100);
		mPlayerNode->setPosition(-100,5,-100);
	}
}
