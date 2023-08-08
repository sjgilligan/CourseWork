//
// Created by Sebbie on 11/21/2022.
//

#include "Model.h"

Model::Model() {
    time = 0;

    Trainer *T1 = new Trainer("Ash", 1, 'T', 1, Point2D(5, 1));
    object_ptrs[0] = T1;
    trainer_ptrs[0] = T1;

    Trainer *T2 = new Trainer("Misty", 2, 'T', 2, Point2D(10, 1));
    object_ptrs[1] = T2;
    trainer_ptrs[1] = T2;

    PokemonCenter *C1 = new PokemonCenter(1, 1, 100, Point2D(1, 20));
    object_ptrs[2] = C1;
    center_ptrs[0] = C1;

    PokemonCenter *C2 = new PokemonCenter(2, 2, 200, Point2D(10, 20));
    object_ptrs[3] = C2;
    center_ptrs[1] = C2;

    PokemonGym *G1 = new PokemonGym(10, 1, 2, 3, 1, Point2D(0, 0));
    object_ptrs[4] = G1;
    gym_ptrs[0] = G1;

    PokemonGym *G2 = new PokemonGym(20, 5, 7.5, 4, 2, Point2D(5, 5));
    object_ptrs[5] = G2;
    gym_ptrs[1] = G2;

    num_centers = 2;
    num_gyms = 2;
    num_trainers = 2;
    num_objects = 6;

    cout << "Model default constructed" << endl;
}

Model::~Model() {
    delete object_ptrs[0];
    delete object_ptrs[1];
    delete object_ptrs[2];
    delete object_ptrs[3];
    delete object_ptrs[4];
    delete object_ptrs[5];
    delete trainer_ptrs[0];
    delete trainer_ptrs[1];
    delete center_ptrs[0];
    delete center_ptrs[1];
    delete gym_ptrs[0];
    delete gym_ptrs[1];
    cout << "Model destructed";
}

Trainer *Model::GetTrainerPtr(int id) {
    /*for (int i = 0; i < 2; i++) {
        if (id == trainer_ptrs[i]->GetId()) {
            return trainer_ptrs[i];
        }
    }*/
    return trainer_ptrs[id - 1];
    //return 0;
}

PokemonCenter *Model::GetPokemonCenterPtr(int id) {
    /*for (int i = 0; i < 2; i++) {
        if (id == center_ptrs[i]->GetId()) {
            return center_ptrs[i];
        }
    }*/
    return center_ptrs[id - 1];
    return 0;
}

PokemonGym *Model::GetPokemonGymPtr(int id) {
    /*for (int i = 0; i < 2; i++) {
        if (id == gym_ptrs[i]->GetId()) {
            return gym_ptrs[i];
        }
    }*/
    return gym_ptrs[id - 1];

}

bool Model::Update() {
    time++;
    //for (int i = 0; i < (sizeof(object_ptrs) / sizeof(object_ptrs[0])); i++)
    for (int i = 0; i < 6; i++) {
        if (object_ptrs[i]->Update()) {
            return true;
        }
    }
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
    for (int i = 0; i < 6; i++) {
        view.Plot(object_ptrs[i]);
    }
}

void Model::ShowStatus() {
    cout << "Time: " << time << endl;
    //for (int i = 0; i < (sizeof(object_ptrs) / sizeof(object_ptrs[0])); i++) {
    for (int i = 0; i < 10; i++) {
        object_ptrs[i]->ShowStatus();
    }
}
