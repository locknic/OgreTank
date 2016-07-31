#include "AIManager.h"
#include "World.h"
#include "OgreSceneNode.h"
#include <OgreEntity.h>

AIManager::AIManager(World *world) : mWorld(world)
{
    // Any initialization that needs to be done ...
}

AIManager::~AIManager()
{
    // Clean up after yourself ...
}

void 
AIManager::Think(float time)
{
	const float PADDLE_SPEED = 40;

}

