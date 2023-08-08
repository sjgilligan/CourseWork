//
// Created by Sebbie on 11/21/2022.
//

#ifndef PA3_MODEL_H
#define PA3_MODEL_H

#include "GameObject.h"
#include "Trainer.h"
#include "WildPokemon.h"
#include "View.h"
#include <list>
#include <iterator>

using namespace std;

class Model {
private:
    int time;
    //GameObject *object_ptrs[10];
    list<GameObject *> object_ptrs;
    list<GameObject *> active_ptrs;
    int num_objects;
    //Trainer *trainer_ptrs[10];
    list<Trainer *> trainer_ptrs;
    int num_trainers;
    //PokemonCenter *center_ptrs[10];
    list<PokemonCenter *> center_ptrs;
    int num_centers;
    //PokemonGym *gym_ptrs[10];
    list<PokemonGym *> gym_ptrs;
    int num_gyms;
    //WildPokemon *wild_ptrs[10];
    list<WildPokemon *> wild_ptrs;
    int num_wild;
public:

    Model();

    ~Model();

    Trainer *GetTrainerPtr(int id);

    PokemonCenter *GetPokemonCenterPtr(int id);

    PokemonGym *GetPokemonGymPtr(int id);

    WildPokemon *GetWildPokemonPtr(int id);

    GameObject *GetGameObjectPtr(int id);

    bool Update();

    void Display(View &view);

    void ShowStatus();

    void RemoveFromActiveList(int);

    bool TrainerAndPokemonInteraction();

    void AddGameObject();

    void AddTrainer(int id, Point2D loc);

    void AddCenter(int id, Point2D loc);

    void AddGym(int id, Point2D loc);

    void AddWild(int id, Point2D loc);

    int GetNumOfObj();

    bool CheckNewObjId(int id, char type);
};


#endif //PA3_MODEL_H
