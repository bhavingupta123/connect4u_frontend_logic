
#ifndef __FIRST_SCENE_H__
#define __FIRST_SCENE_H__

#include "axmol.h"

class FirstScene : public ax::Scene {
public:
    static FirstScene* createScene();
    virtual bool init();
    void promptForNameThenStartOnline();

    CREATE_FUNC(FirstScene);

private:
    void promptForNameThenShowStats();
};

#endif // __FIRST_SCENE_H__
