#include "GameCommand.h"

void DoMoveCommand(Model &model, int trainer_id, Point2D p1) {
    if (model.GetTrainerPtr(trainer_id)) {
        cout << "Moving " << model.GetTrainerPtr(trainer_id)->GetName() << " to " << p1 << endl;
        model.GetTrainerPtr(trainer_id)->StartMoving(p1);
    } else {
        cout << "Error: Please enter a valid command!" << endl;
    }
}

void DoMoveToCenterCommand(Model &model, int trainer_id, int center_id) {
    if (model.GetTrainerPtr(trainer_id) && model.GetPokemonCenterPtr(center_id)) {
        cout << "Moving " << model.GetTrainerPtr(trainer_id)->GetName() << " to Pokemon Center "
             << model.GetPokemonCenterPtr(center_id)->GetId() << endl;
        model.GetTrainerPtr(trainer_id)->StartMovingToCenter(model.GetPokemonCenterPtr(center_id));
    } else {
        cout << "Error: Please enter a valid command!" << endl;
    }
}

void DoMoveToGymCommand(Model &model, int trainer_id, int gym_id) {
    if (model.GetTrainerPtr(trainer_id)/* && model.GetPokemonGymPtr(gym_id)*/) {
        cout << "Moving " << model.GetTrainerPtr(trainer_id)->GetName() << " to Pokemon Gym "
             << model.GetPokemonGymPtr(gym_id)->GetId() << endl;
        model.GetTrainerPtr(trainer_id)->StartMovingToGym(model.GetPokemonGymPtr(gym_id));
    } else {
        cout << "Error: Please enter a valid command!" << endl;
    }
}

void DoStopCommand(Model &model, int trainer_id) {
    if (model.GetTrainerPtr(trainer_id)) {
        cout << "Stopping " << model.GetTrainerPtr(trainer_id)->GetName() << endl;
        model.GetTrainerPtr(trainer_id)->Stop();
    } else {
        cout << "Error: Please enter a valid command!" << endl;
    }
}

void DoBattleCommand(Model &model, int trainer_id, unsigned int battles) {
    if (model.GetTrainerPtr(trainer_id)) {
        cout << model.GetTrainerPtr(trainer_id)->GetName() << " is battling" << endl;
        model.GetTrainerPtr(trainer_id)->StartBattling(battles);
    } else {
        cout << "Error: Please enter a valid command!" << endl;
    }
}

void DoRecoverInCenterCommand(Model &model, int trainer_id, unsigned int potions_needed) {
    if (model.GetTrainerPtr(trainer_id)) {
        cout << "Recovering " << model.GetTrainerPtr(trainer_id)->GetName() << "'s Pokemon's health" << endl;
        model.GetTrainerPtr(trainer_id)->StartRecoveringHealth(potions_needed);
    } else {
        cout << "Error: Please enter a valid command!" << endl;
    }
}

void DoAdvanceCommand(Model &model, View &view) {
    cout << "Advancing one tick" << endl;
    model.Update();
    model.Display(view);
}

void DoRunCommand(Model &model, View &view) {
    cout << "Advancing to next event" << endl;
    int run_num = 0;
    while (run_num < 5) {
        if (model.Update()) {
            run_num = 5;
        }
        run_num++;
    }
    model.Display(view);
}


