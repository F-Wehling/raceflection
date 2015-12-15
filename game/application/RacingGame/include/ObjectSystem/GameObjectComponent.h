#pragma once

BEGINNAMESPACE
class GameObject;
class GameObjectComponent{
public:
    virtual ~GameObjectComponent(){};

    virtual bool process(float32 dt, GameObject* gameObject) = 0;
};

ENDNAMESPACE
