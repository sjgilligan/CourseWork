//
// Created by Sebbie on 11/18/2022.
//

#include "Trainer.h"
#include <math.h>
#include <random>

Trainer::Trainer() {
    speed = 5;
    experience = 0;
    is_at_center = false;
    is_IN_GYM = false;
    health = 20;
    PokeDollars = 0;
    battles_to_buy = 0;
    potions_to_buy = 0;
    current_gym = NULL;
    current_center = NULL;
    cout << "Trainer default constructed" << endl;
}

Trainer::Trainer(char in_code) {
    experience = 0;
    is_at_center = false;
    is_IN_GYM = false;
    health = 20;
    PokeDollars = 0;
    battles_to_buy = 0;
    potions_to_buy = 0;
    current_gym = NULL;
    current_center = NULL;
    speed = 5;
    state = STOPPED;
    display_code = in_code;
    cout << "Trainer constructed" << endl;
}

Trainer::Trainer(string in_name, int in_id, char in_code, unsigned int in_speed, Point2D in_loc) {
    speed = in_speed;
    experience = 0;
    is_at_center = false;
    is_IN_GYM = false;
    health = 20;
    PokeDollars = 0;
    battles_to_buy = 0;
    potions_to_buy = 0;
    current_gym = NULL;
    current_center = NULL;
    name = in_name;
    id_num = in_id;
    display_code = in_code;
    location = in_loc;
    cout << "Trainer constructed" << endl;
}

Trainer::~Trainer() {
    cout << "Trainer destructed" << endl;
}

void Trainer::StartMoving(Point2D dest) {
    destination = dest;
    SetupDestination(dest);
    state = MOVING;
    if (location == dest) {
        cout << display_code << id_num << ": I’m already there. See?" << endl;
    } else if (state == FAINTED) {
        cout << display_code << id_num << ": My pokemon have fainted. I may move but you cannot see me."
             << endl;
    } else {
        cout << display_code << id_num << ": On my way." << endl;
    }
}

void Trainer::StartMovingToGym(PokemonGym *gym) {
    current_gym = gym;
    destination = current_gym->GetLocation();
    SetupDestination(destination);
    state = MOVING_TO_GYM;
    if (location == current_gym->GetLocation()) {
        cout << display_code << id_num << ": I'm already at the PokemonGym" << endl;
    }
    if (state == FAINTED) {
        cout << display_code << id_num << ": My pokemon have fainted so I can't move to a gym." << endl;
    } else {
        cout << display_code << id_num << ": On my way to gym " << gym->GetId() << endl;
    }
}

void Trainer::StartMovingToCenter(PokemonCenter *center) {
    current_center = center;
    destination = current_center->GetLocation();
    SetupDestination(destination);
    state = MOVING_TO_CENTER;
    if (location == current_center->GetLocation()) {
        cout << display_code << id_num << ": I'm already at the PokemonCenter" << endl;
    }
    if (state == FAINTED) {
        cout << display_code << id_num << ": My pokemon have fainted so I can't move to a center."
             << endl;
    } else {
        cout << display_code << id_num << ": On my way to center " << center->GetId() << endl;
    }
}

void Trainer::StartBattling(unsigned int num_battles) {
    battles_to_buy = num_battles;
    if (!is_IN_GYM) {
        cout << display_code << id_num << ": I can only battle in a PokemonGym!" << endl;
    } else if (PokeDollars < (current_gym->GetPokeDollarCost(num_battles))) {
        cout << display_code << id_num << ": Not enough money for battles" << endl;
    } else if (state == FAINTED) {
        cout << display_code << id_num << ": My Pokemon have fainted so no more battles for me..."
             << endl;
    } else if (current_gym->GetNumBattlesRemaining() == 0) {
        cout << "(" << display_code << ")(" << id_num
             << "): Cannot battle! This PokemonGym has no more trainers to battle!" << endl;
    } else {
        state = BATTLING_IN_GYM;
        experience = experience + current_gym->TrainPokemon(num_battles);
        health = health - current_gym->GetHealthCost(num_battles);
        PokeDollars = PokeDollars - current_gym->GetPokeDollarCost(num_battles);
        cout << "(" << display_code << ") Started to battle at the PokemonGym (" << current_gym->GetId() << ") with "
             << num_battles << " battles" << endl;
    }
    //If this Trainer can start training, set its num_battles to the requested battles and
    //update the remaining battles in the gym . This will be used when its Update()
    //function is called.
}

void Trainer::StartRecoveringHealth(unsigned int num_potions) {
    potions_to_buy = num_potions;
    if (!is_at_center) {
        cout << display_code << id_num << ": I can only recover health at a Pokemon Center!" << endl;
    } else if (!current_center->CanAffordPotion(num_potions, PokeDollars)) {
        cout << display_code << id_num << ": Not enough money to recover health" << endl;
    } else if (!current_center->HasPotions()) {
        cout << display_code << id_num << ": Cannot recover! No potion remaining in this Pokemon Center"
             << endl;
    } else {
        state = RECOVERING_HEALTH;
        cout << display_code << id_num << ": Started recovering " << num_potions
             << " potions at Pokemon Center " << current_center->GetId() << endl;
        current_center->DistributePotion(num_potions);
        health = health + (num_potions * 5);
        PokeDollars = PokeDollars - current_center->GetPokeDollarCost(num_potions);
        //If this Trainer can start recovering health, set its potions_to_buy to the minimum
        //of the requested potions and update the remaining potions in the center. This will
        //be used when its Update() function is called.

        //Five health is recovered for each potion purchased
    }
}

void Trainer::Stop() {
    state = STOPPED;
    cout << display_code << id_num << ": Stopping..." << endl;
}

bool Trainer::HasFainted() {
    if (this->health == 0) {
        return true;
    } else {
        return false;
    }
}

bool Trainer::ShouldBeVisible() {
    if (!HasFainted()) {
        return true;
    } else {
        return false;
    }
}

void Trainer::ShowStatus() {
    cout << name << " Status:" << endl;
    GameObject::ShowStatus();
    cout << "State:";
    switch (state) {
        case 0:
            cout << " stopped" << endl;
            break;
        case 1:
            cout << " moving at a speed of " << speed << " to destination " << destination << " at each step of "
                 << delta << endl;
            break;
        case 6:
            cout << " heading to PokemonGym " << current_gym->GetId() << " at a speed of " << speed
                 << " at each step of " << delta << endl;
            break;
        case 5:
            cout << " heading to Pokemon Center " << current_center->GetId() << " at a speed of " << speed
                 << " at each step of " << delta << endl;
            break;
        case 4:
            cout << " inside PokemonGym " << current_gym->GetId() << endl;
            break;
        case 3:
            cout << " inside Pokemon Center " << current_center->GetId() << endl;
            break;
        case 7:
            cout << " battling in PokemonGym " << current_gym->GetId() << endl;
        case 8:
            cout << " recovering health in Pokemon Center " << current_center->GetId() << endl;
    }
}

bool Trainer::Update() {
    if (health <= 0) {
        state = FAINTED;
    }
    switch (state) {
        case 0:
            cout << "Trainer " << id_num << ":" << endl;
            cout << "Health: " << health << endl;
            cout << "PokeDollars: " << PokeDollars << endl;
            cout << "Experience: " << experience << endl;
            return false;
        case 1:
            UpdateLocation();
            health--;
            PokeDollars = PokeDollars + GetRandomAmountOfPokeDollar();
            cout << "Trainer " << id_num << ":" << endl;
            cout << "Health: " << health << endl;
            cout << "PokeDollars: " << PokeDollars << endl;
            cout << "Experience: " << experience << endl;
            if (location == destination) {
                state = STOPPED;
                return true;
            } else {
                state = MOVING;
                return false;
            }
        case 6:
            if (is_at_center) {
                is_at_center = false;
                current_center->RemoveOneTrainer();
            }
            if (UpdateLocation()) {
                is_IN_GYM = true;
                current_gym->AddOneTrainer();
            }
            health--;
            PokeDollars = PokeDollars + GetRandomAmountOfPokeDollar();
            cout << "Trainer " << id_num << ":" << endl;
            cout << "Health: " << health << endl;
            cout << "PokeDollars: " << PokeDollars << endl;
            cout << "Experience: " << experience << endl;
            if (is_IN_GYM) {
                state = IN_GYM;
                return true;
            } else {
                state = MOVING_TO_GYM;
                return false;
            }
        case 5:
            if (is_IN_GYM) {
                is_IN_GYM = false;
                current_gym->RemoveOneTrainer();
            }
            if (UpdateLocation()) {
                is_at_center = true;
                current_center->AddOneTrainer();
            }
            health--;
            PokeDollars = PokeDollars + GetRandomAmountOfPokeDollar();
            cout << "Trainer " << id_num << ":" << endl;
            cout << "Health: " << health << endl;
            cout << "PokeDollars: " << PokeDollars << endl;
            cout << "Experience: " << experience << endl;
            if (is_at_center) {
                state = AT_CENTER;
                return true;
            } else {
                state = MOVING_TO_CENTER;
                return false;
            }
        case 4:
        case 3:
            cout << "Trainer " << id_num << ":" << endl;
            cout << "Health: " << health << endl;
            cout << "PokeDollars: " << PokeDollars << endl;
            cout << "Experience: " << experience << endl;
            return false;
        case 7:
            health = health - current_gym->GetHealthCost(battles_to_buy);
            PokeDollars = PokeDollars - current_gym->GetPokeDollarCost(battles_to_buy);
            experience = experience + current_gym->TrainPokemon(battles_to_buy);
            cout << "** " << name << " completed " << battles_to_buy << " battle(s)! **" << endl;
            cout << "** " << name << " gained " << current_gym->TrainPokemon(battles_to_buy) << " experience! **"
                 << endl;
            state = IN_GYM;
            cout << "Trainer " << id_num << ":" << endl;
            cout << "Health: " << health << endl;
            cout << "PokeDollars: " << PokeDollars << endl;
            cout << "Experience: " << experience << endl;
            return true;
        case 8:
            PokeDollars = PokeDollars - current_center->GetPokeDollarCost(potions_to_buy);
            cout << "** " << name << " recovered " << potions_to_buy * 5 << " health! **" << endl;
            cout << "** " << name << " bought " << potions_to_buy << " potion(s)! **" << endl;
            state = AT_CENTER;
            cout << "Trainer " << id_num << ":" << endl;
            cout << "Health: " << health << endl;
            cout << "PokeDollars: " << PokeDollars << endl;
            cout << "Experience: " << experience << endl;
            return true;
        case 2:
            cout << name << " is out of health and cannot move";
            cout << "Trainer " << id_num << ":" << endl;
            cout << "Health: " << health << endl;
            cout << "PokeDollars: " << PokeDollars << endl;
            cout << "Experience: " << experience << endl;
        default:
            return false;
    }
}

bool Trainer::UpdateLocation() {
    if ((destination - location) <= delta) {
        //if (destination == location) {
        location = destination;
        cout << "arrived" << endl;
        return true;
    } else {
        location = location + delta;
        cout << "moved" << endl;
        return false;
    }
}

void Trainer::SetupDestination(Point2D dest) {
    delta = (destination - location) * (speed / GetDistanceBetween(destination, location));
}

static double GetRandomAmountOfPokeDollar() {
    uniform_real_distribution<double> unif(0.0, 2.0);
    default_random_engine re;
    return unif(re);
}

string Trainer::GetName() {
    return name;
}

void Trainer::GainPokemon(WildPokemon *pokemon) {
    cout << "Wild Pokemon " << pokemon->get_name() << " is now following " << this->name << endl;
    health = health * pokemon->get_health();

}

Vector2D Trainer::GetDelta() {
    return delta;
}