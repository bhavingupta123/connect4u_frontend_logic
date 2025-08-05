
#ifndef __FIRST_SCENE_H__
#define __FIRST_SCENE_H__

#include "axmol.h"

class FirstScene : public ax::Scene {
public:
    static FirstScene* createScene();
    virtual bool init();

    CREATE_FUNC(FirstScene);
};

#endif // __FIRST_SCENE_H__
