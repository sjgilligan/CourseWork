//
// Created by Sebbie on 11/29/2022.
//

#ifndef PA3_WILDPOKEMON_H
#define PA3_WILDPOKEMON_H

class Trainer;

#include "GameObject.h"


class WildPokemon : public GameObject {
    enum WildPokemonStates {
        IN_ENVIRONMENT = 0,
        DEAD = 1,
        IN_TRAINER = 2
    };
public:
    WildPokemon(string name, double attack, double health, bool variant, int id,
                Point2D in_loc);//:GameObject('W', id, in_loc);
    void follow(Trainer *t);

    bool get_variant();

    double get_attack();

    double get_health();

    bool get_in_combat();

    string get_name();

    bool Update();

    void ShowStatus();

    bool ShouldBeVisible();

    bool IsAlive();

    Vector2D range;

protected:
    double attack;
    double health;
    bool variant;
    bool in_combat;
    string name;
    Trainer *current_trainer;
};


#endif //PA3_WILDPOKEMON_H
