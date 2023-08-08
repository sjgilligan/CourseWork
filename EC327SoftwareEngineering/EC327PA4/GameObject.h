#ifndef PA3_GAMEOBJECT_H
#define PA3_GAMEOBJECT_H


#include "Point2D.h"

class GameObject {
protected:
    Point2D location;
    int id_num;
    char display_code;
    char state;
public:
    GameObject();

    GameObject(char);

    GameObject(Point2D, int, char);

    ~GameObject();

    Point2D GetLocation();

    int GetId();

    char *GetDisplayCode();

    char GetState();

    virtual void ShowStatus();

    virtual bool ShouldBeVisible() = 0;

    virtual bool Update() = 0;

    void DrawSelf(char *ptr);
};


#endif //PA3_GAMEOBJECT_H
