//
// Created by Sebbie on 11/15/2022.
//

#include "PokemonCenter.h"

PokemonCenter::PokemonCenter() {
    display_code = 'C';
    potion_capacity = 100;
    num_potions_remaining = potion_capacity;
    pokedollar_cost_per_potion = 5;
    state = POTIONS_AVAILABLE;
    cout << "PokemonCenter default constructed" << endl;
}

PokemonCenter::PokemonCenter(int in_id, double potion_cost, unsigned int potion_cap, Point2D in_loc) {
    display_code = 'C';
    location = in_loc;
    id_num = in_id;
    pokedollar_cost_per_potion = potion_cost;
    potion_capacity = potion_cap;
    num_potions_remaining = potion_capacity;
    state = POTIONS_AVAILABLE;
    cout << "PokemonCenter constructed" << endl;
}

bool PokemonCenter::HasPotions() {
    if (num_potions_remaining >= 1) {
        return true;
    } else {
        return false;
    }
}

unsigned int PokemonCenter::GetNumPotionRemaining() {
    return num_potions_remaining;
}

bool PokemonCenter::CanAffordPotion(unsigned int potion, double budget) {
    //Returns true if this Trainer can afford to purchase potion with the given budget.
    if ((potion * this->pokedollar_cost_per_potion) > budget) {
        return false;
    } else {
        return true;
    }
}

double PokemonCenter::GetPokeDollarCost(unsigned int potion) {
    return (potion * pokedollar_cost_per_potion);
}

unsigned int PokemonCenter::DistributePotion(unsigned int potion_needed) {
    if (num_potions_remaining >= potion_needed) {
        num_potions_remaining -= potion_needed;
        return potion_needed;
    } else {
        return num_potions_remaining;
        num_potions_remaining = 0;
    }
}

bool PokemonCenter::Update() {
    static int first_time_happened = false;
    if (num_potions_remaining == 0 && first_time_happened == false) {
        state = NO_POTIONS_AVAILABLE;
        display_code = 'c';
        cout << "PokemonCenter (" << id_num << ") has run out of potions" << endl;
        first_time_happened = true;
        return true;
    } else if (num_potions_remaining == 0 && first_time_happened == true) {
        state = NO_POTIONS_AVAILABLE;
        display_code = 'c';
        cout << "PokemonCenter (" << id_num << ") has run out of potions" << endl;
        return false;
    } else {
        return false;
    }
}

void PokemonCenter::ShowStatus() {
    cout << "PokemonCenter Status " << endl;
    Building::ShowStatus();
    cout << "PokeDollars per potion: " << pokedollar_cost_per_potion << endl;
    cout << "This PokemoneCenter has " << num_potions_remaining << " potion(s) remaining" << endl;
}

PokemonCenter::~PokemonCenter() {
    cout << "PokemonCenter destructed" << endl;
}
