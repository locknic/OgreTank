#ifndef __World_h_
#define __World_h_

#include <vector>
// Forward delcarations of Ogre classes.  Note the Ogre namespace!
namespace Ogre {
    class SceneNode;
    class SceneManager;
	class Entity;
	class Overlay;
}

// Forward delcarations of our own classes
class PongCamera;
class InputHandler;


class World
{
public:
	
    World(Ogre::SceneManager *sceneManager, InputHandler *input);

    // You'll want various methods to access & change your world here
    
    void Think(float time);
	
	void addCamera(PongCamera *c) { mCamera = c; }

	void score(bool playerScored);

	Ogre::SceneManager *SceneManager() { return mSceneManager; }

	Ogre::Entity *getUpperBoundsEntity() { return mUpperBoundsEntity; }
	Ogre::Entity *getLowerBoundsEntity() { return mLowerBoundsEntity; }
	Ogre::Entity *getLeftBoundsEntity() { return mLeftBoundsEntity; }
	Ogre::Entity *getRightBoundsEntity() { return mRightBoundsEntity; }

	Ogre::Entity *getPlayerEntity() { return mPlayerEntity; }
	Ogre::SceneNode *getPlayerNode() { return mPlayerNode; }

	Ogre::Entity *getComputerEntity() { return mComputerEntity; }
	Ogre::SceneNode *getComputerNode() { return mComputerNode; }
	
	std::vector<Ogre::Entity*> *getCollisionList() { return mCollisionList;}

	float getSpeed() { return mSpeed; }
	void setSpeed(float speed){ mSpeed = speed; }

protected:

	Ogre::SceneManager *mSceneManager;

	InputHandler *mInputHandler;
	PongCamera *mCamera;

	Ogre::Entity *mUpperBoundsEntity;
	Ogre::Entity *mLowerBoundsEntity;
	Ogre::Entity *mLeftBoundsEntity;
	Ogre::Entity *mRightBoundsEntity;

	Ogre::Entity *mPlayerEntity;
	Ogre::SceneNode *mPlayerNode;

	Ogre::Entity *mBulletEntity;
	Ogre::SceneNode *mBulletNode;
	double reloadTimer;
	bool aliveBullet;

	Ogre::Entity *mBulletEntity2;
	Ogre::SceneNode *mBulletNode2;
	double reloadTimer2;
	bool aliveBullet2;

	Ogre::Entity *mComputerEntity;
	Ogre::SceneNode *mComputerNode;

	std::vector<Ogre::Entity*> *mCollisionList;

	int playerScore;
	int computerScore;
	float mSpeed;
	float mSize;

};

#endif