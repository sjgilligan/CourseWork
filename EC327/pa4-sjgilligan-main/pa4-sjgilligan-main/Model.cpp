//
// Created by Sebbie on 11/21/2022.
//

#include "Model.h"

Model::Model() {
    time = 0;

    Trainer *T1 = new Trainer("Ash", 1, 'T', 1, Point2D(5, 1));
    active_ptrs.push_back(T1);
    object_ptrs.push_back(T1);
    trainer_ptrs.push_back(T1);

    Trainer *T2 = new Trainer("Misty", 2, 'T', 2, Point2D(10, 1));
    active_ptrs.push_back(T2);
    object_ptrs.push_back(T2);
    trainer_ptrs.push_back(T2);

    PokemonCenter *C1 = new PokemonCenter(1, 1, 100, Point2D(1, 20));
    active_ptrs.push_back(C1);
    object_ptrs.push_back(C1);
    center_ptrs.push_back(C1);

    PokemonCenter *C2 = new PokemonCenter(2, 2, 200, Point2D(10, 20));
    active_ptrs.push_back(C2);
    object_ptrs.push_back(C2);
    center_ptrs.push_back(C2);

    PokemonGym *G1 = new PokemonGym(10, 1, 2, 3, 1, Point2D(0, 0));
    active_ptrs.push_back(G1);
    object_ptrs.push_back(G1);
    gym_ptrs.push_back(G1);

    PokemonGym *G2 = new PokemonGym(20, 5, 7.5, 4, 2, Point2D(5, 5));
    active_ptrs.push_back(G2);
    object_ptrs.push_back(G2);
    gym_ptrs.push_back(G2);

    WildPokemon *W1 = new WildPokemon("Pikachu", 5, 2, false, 1, Point2D(10, 12));
    active_ptrs.push_back(W1);
    object_ptrs.push_back(W1);
    wild_ptrs.push_back(W1);

    WildPokemon *W2 = new WildPokemon("Dragonite", 5, 2, false, 2, Point2D(15, 5));
    active_ptrs.push_back(W2);
    object_ptrs.push_back(W2);
    wild_ptrs.push_back(W2);

    num_centers = 2;
    num_gyms = 2;
    num_trainers = 2;
    num_wild = 2;
    num_objects = 8;

    cout << "Model default constructed" << endl;
}

Model::~Model() {
    /*delete object_ptrs[0];
    delete object_ptrs[1];
    delete object_ptrs[2];
    delete object_ptrs[3];
    delete object_ptrs[4];
    delete object_ptrs[5];*/
    list<GameObject *>::iterator itO;
    for (itO = object_ptrs.begin(); itO == object_ptrs.end(); ++itO) {
        delete *itO;
    }
    /*list<Trainer *>::iterator itT;
    for (itT = trainer_ptrs.begin(); itT == trainer_ptrs.end(); ++itT) {
        delete *itT;
    }
    list<PokemonCenter *>::iterator itC;
    for (itC = center_ptrs.begin(); itC == center_ptrs.end(); ++itC) {
        delete *itC;
    }
    list<PokemonGym *>::iterator itG;
    for (itG = gym_ptrs.begin(); itG == gym_ptrs.end(); ++itG) {
        delete *itG;
    }
    list<WildPokemon *>::iterator itW;
    for (itW = wild_ptrs.begin(); itW == wild_ptrs.end(); ++itW) {
        delete *itW;
    }*/

    /* delete trainer_ptrs[0];
     delete trainer_ptrs[1];
     delete center_ptrs[0];
     delete center_ptrs[1];
     delete gym_ptrs[0];
     delete gym_ptrs[1];*/
    cout << "Model destructed";
}

Trainer *Model::GetTrainerPtr(int id) {
    list<Trainer *>::iterator it;
    it = trainer_ptrs.begin();
    for (int i = 0; i < (id - 1); i++) {
        ++it;
    }
    return *it;
}

PokemonCenter *Model::GetPokemonCenterPtr(int id) {
    list<PokemonCenter *>::iterator it = center_ptrs.begin();
    for (int i = 0; i < (id - 1); i++) {
        ++it;
    }
    return *it;
}

PokemonGym *Model::GetPokemonGymPtr(int id) {
    list<PokemonGym *>::iterator it;
    it = gym_ptrs.begin();
    for (int i = 0; i < (id - 1); i++) {
        ++it;
    }
    return *it;
}

WildPokemon *Model::GetWildPokemonPtr(int id) {
    list<WildPokemon *>::iterator it;
    it = wild_ptrs.begin();
    for (int i = 0; i < (id - 1); i++) {
        ++it;
    }
    return *it;
}

GameObject *Model::GetGameObjectPtr(int id) {
    list<GameObject *>::iterator it;
    it = object_ptrs.begin();
    for (int i = 0; i < (id - 1); i++) {
        ++it;
    }
    return *it;
}

bool Model::Update() {
    time++;
    bool update = false;
    //for (int i = 0; i < (sizeof(object_ptrs) / sizeof(object_ptrs[0])); i++)

    for (int i = 0; i < 8; i++) {
        if (GetGameObjectPtr(i)->Update()) {
            update = true;
        }
    }
    for (int i = 0; i < 8; i++) {
        if (!GetGameObjectPtr(i)->ShouldBeVisible()) {
            RemoveFromActiveList(i);
        }
    }
    //for (int k = 0; k < 8; k++)
    for (int i = 1; i <= 2; i++) {
        for (int j = 1; j <= 2; j++) {
            if (abs(this->GetTrainerPtr(i)->GetLocation().x - this->GetWildPokemonPtr(j)->GetLocation().x) < 2
                && abs(this->GetTrainerPtr(i)->GetLocation().y - this->GetWildPokemonPtr(j)->GetLocation().y) < 2) {
                //cout << "trainer and wild on same location" << endl;
                this->GetWildPokemonPtr(j)->follow(GetTrainerPtr(i));
                this->GetTrainerPtr(i)->GainPokemon(GetWildPokemonPtr(j));
            }
        }
    }
    return update;
    /*int num_of_gym_passed = 0;
    int num_of_gym_filled = 0;
    for (int i = 0; i < 2; i++) {
        if (gym_ptrs[i]->passed()) {
            num_of_gym_passed++;
        }
        if (gym_ptrs[i] != nullptr) {
            num_of_gym_filled++;
        }
    }
    if (num_of_gym_passed == num_of_gym_filled) {
        cout << "GAME OVER: You win! All battles done!" << endl;
        exit;
    }
    int num_of_trainer_fainted = 0;
    int num_of_trainer_filled = 0;
    for (int i = 0; i < 2; i++) {
        if (trainer_ptrs[i]->HasFainted()) {
            num_of_trainer_fainted++;
        }
        if (gym_ptrs[i] != nullptr) {
            num_of_trainer_filled++;
        }
    }
    if (num_of_trainer_fainted == num_of_trainer_filled) {
        cout << "GAME OVER: You lose! All of your Trainers' pokemon have fainted!";
        exit;
    }*/
    return false;
}

void Model::Display(View &view) {
    cout << "Time: " << time << endl;
    for (int i = 0; i < 8; i++) {
        view.Plot(GetGameObjectPtr(i));
    }
}

void Model::ShowStatus() {
    cout << "Time: " << time << endl;
    //for (int i = 0; i < (sizeof(object_ptrs) / sizeof(object_ptrs[0])); i++) {
    for (int i = 0; i < 10; i++) {
        GetGameObjectPtr(i)->ShowStatus();
    }
}

bool Model::TrainerAndPokemonInteraction() {
    for (int i = 0; i < 1; i++) {
        if (GetTrainerPtr(i)->GetLocation() == GetTrainerPtr(i)->GetLocation()) {
            cout << "trainer and wild on same location";
            return true;
        }
    }
    return false;
}

void Model::RemoveFromActiveList(int id) {
    list<GameObject *>::iterator it = active_ptrs.begin();
    for (int i = 0; i < id; i++) {
        ++it;
    }
    cout << "Dead Object Removed" << endl;
    active_ptrs.erase(it);
}

void Model::AddCenter(int id, Point2D loc) {
    auto *newCenter = new PokemonCenter(id, 1, 100, loc);
    active_ptrs.push_back(newCenter);
    object_ptrs.push_back(newCenter);
    center_ptrs.push_back(newCenter);
}

void Model::AddGym(int id, Point2D loc) {
    auto *newGym = new PokemonGym(10, 1, 2, 3, id, loc);
    active_ptrs.push_back(newGym);
    object_ptrs.push_back(newGym);
    gym_ptrs.push_back(newGym);
}

void Model::AddTrainer(int id, Point2D loc) {
    auto *newTrainer = new Trainer("RandomTrainer", id, 'T', 1, loc);
    active_ptrs.push_back(newTrainer);
    object_ptrs.push_back(newTrainer);
    trainer_ptrs.push_back(newTrainer);
}

void Model::AddWild(int id, Point2D loc) {
    auto *newWild = new WildPokemon("Random", 5, 2, false, id, loc);
    active_ptrs.push_back(newWild);
    object_ptrs.push_back(newWild);
    wild_ptrs.push_back(newWild);
}

int Model::GetNumOfObj() {
    return object_ptrs.size();
}

bool Model::CheckNewObjId(int id, char type) {
    switch (type) {
        case 'c':
            break;
        case 'g':

            break;
        case 't':

            break;
        case 'w':

            break;
        default:

            break;
    }
}