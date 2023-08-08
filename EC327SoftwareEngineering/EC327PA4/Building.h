#ifndef PA3_BUILDING_H
#define PA3_BUILDING_H

#include "GameObject.h"

class Building : public GameObject {
private:
    unsigned int trainer_count;
public:
    Building();

    Building(char, int, Point2D);

    void AddOneTrainer();

    void RemoveOneTrainer();

    void ShowStatus();

    bool ShouldBeVisible();
};


#endif //PA3_BUILDING_H
