//
// Created by Sebbie on 11/29/2022.
//
using namespace std;

#include "WildPokemon.h"
//#include "Trainer.h"
#include "Model.h"

WildPokemon::WildPokemon(string in_name, double attack, double in_health, bool variant, int in_id, Point2D in_loc) {
    location = in_loc;
    name = in_name;
    id_num = in_id;
    display_code = 'W';
    attack = 5;
    health = in_health;
    variant = false;
    in_combat = false;
    state = IN_ENVIRONMENT;
    range = Vector2D(5, 5);
    current_trainer = nullptr;
}

void WildPokemon::follow(Trainer *t) {
    current_trainer = t;
    in_combat = true;
    state = IN_TRAINER;
}

double WildPokemon::get_attack() {
    return attack;
}

double WildPokemon::get_health() {
    return health;
}

bool WildPokemon::get_variant() {
    return variant;
}

bool WildPokemon::get_in_combat() {
    return in_combat;
}

string WildPokemon::get_name() {
    return name;
}

bool WildPokemon::Update() {
    switch (state) {
        case 0:
            if (health <= 0) {
                state = DEAD;
                return true;
            } else {
                return false;
            }
        case 2:
            location = current_trainer->GetLocation();
            return false;
        default:
            return false;
    }
    return false;
}

void WildPokemon::ShowStatus() {
    cout << "Wild Pokemon Status:" << endl;
    GameObject::ShowStatus();
    switch (state) {
        case 0:
            cout << "In the environment" << endl;
        case 1:
            cout << "Is dead" << endl;
        case 2:
            cout << "In trainer" << endl;
    }
}

bool WildPokemon::IsAlive() {
    if (health <= 0) {
        return false;
    } else {
        return true;
    }
}

bool WildPokemon::ShouldBeVisible() {
    if (current_trainer == nullptr) {
        return true;
    } else {
        return false;
    }
}