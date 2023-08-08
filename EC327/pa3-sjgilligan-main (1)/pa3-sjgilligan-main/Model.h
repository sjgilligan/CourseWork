//
// Created by Sebbie on 11/21/2022.
//

#ifndef PA3_MODEL_H
#define PA3_MODEL_H

#include "GameObject.h"
#include "Trainer.h"
#include "View.h"

class Model {
private:
    int time;
    GameObject *object_ptrs[10];
    int num_objects;
    Trainer *trainer_ptrs[10];
    int num_trainers;
    PokemonCenter *center_ptrs[10];
    int num_centers;
    PokemonGym *gym_ptrs[10];
    int num_gyms;
public:
    Model();

    ~Model();

    Trainer *GetTrainerPtr(int id);

    PokemonCenter *GetPokemonCenterPtr(int id);

    PokemonGym *GetPokemonGymPtr(int id);

    bool Update();

    void Display(View &view);

    void ShowStatus();


};


#endif //PA3_MODEL_H
