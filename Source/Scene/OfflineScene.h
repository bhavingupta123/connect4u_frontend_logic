
#ifndef __OFFLINE_SCENE_H__
#define __OFFLINE_SCENE_H__

#include "axmol.h"

class OfflineScene : public ax::Scene {
public:
    static OfflineScene* createScene();
    virtual bool init();

    CREATE_FUNC(OfflineScene);
};

#endif // __OFFLINE_SCENE_H__
