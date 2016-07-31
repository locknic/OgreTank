#ifndef __AIManager_h_
#define __AIManager_h_

namespace Ogre {
    class SceneNode;
}

class World;

class AIManager 
{

public:
    // You may want to add parameters to the AI Manager constructor
	AIManager(World *world);
	~AIManager();

    // Go through all of the AIs, and call think on each one
    void Think(float time);

protected:
	World *mWorld;
};

#endif