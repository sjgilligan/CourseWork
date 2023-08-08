#include "GameCommand.h"
#include "Input_Handling.h"

void DoMoveCommand(Model &model, int trainer_id, Point2D p1) {
    bool error = true;
    try {
        if (model.GetTrainerPtr(trainer_id)) {
            cout << "Moving " << model.GetTrainerPtr(trainer_id)->GetName() << " to " << p1 << endl;
            model.GetTrainerPtr(trainer_id)->StartMoving(p1);
            error = false;
        } else {
            throw (Invalid_Input("Trainer ID not found"));
        }
    }
    catch (Invalid_Input &except) {
        cout << "Invalid input - " << except.msg_ptr << endl;
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
    try {
        if (model.GetTrainerPtr(trainer_id)/* && model.GetPokemonGymPtr(gym_id)*/) {
            cout << "Moving " << model.GetTrainerPtr(trainer_id)->GetName() << " to Pokemon Gym "
                 << model.GetPokemonGymPtr(gym_id)->GetId() << endl;
            model.GetTrainerPtr(trainer_id)->StartMovingToGym(model.GetPokemonGymPtr(gym_id));
        } else {
            throw (Invalid_Input("Trainer ID not found"));
        }
    }
    catch (Invalid_Input &except) {
        cout << "Invalid input - " << except.msg_ptr << endl;
    }
}

void DoStopCommand(Model &model, int trainer_id) {
    try {
        if (model.GetTrainerPtr(trainer_id)) {
            cout << "Stopping " << model.GetTrainerPtr(trainer_id)->GetName() << endl;
            model.GetTrainerPtr(trainer_id)->Stop();
        } else {
            throw (Invalid_Input("Trainer ID not found"));
        }
    }
    catch (Invalid_Input &except) {
        cout << "Invalid input - " << except.msg_ptr << endl;
    }
}

void DoBattleCommand(Model &model, int trainer_id, unsigned int battles) {
    try {
        if (model.GetTrainerPtr(trainer_id)) {
            cout << model.GetTrainerPtr(trainer_id)->GetName() << " is battling" << endl;
            model.GetTrainerPtr(trainer_id)->StartBattling(battles);
        } else if (model.GetTrainerPtr(trainer_id) == nullptr) {
            throw (Invalid_Input("Trainer ID not found"));
        }
    }
    catch (Invalid_Input &except) {
        cout << "Invalid input - " << except.msg_ptr << endl;
    }
}

void DoRecoverInCenterCommand(Model &model, int trainer_id, unsigned int potions_needed) {
    try {
        if (model.GetTrainerPtr(trainer_id)) {
            cout << "Recovering " << model.GetTrainerPtr(trainer_id)->GetName() << "'s Pokemon's health" << endl;
            model.GetTrainerPtr(trainer_id)->StartRecoveringHealth(potions_needed);
        } else {
            cout << "Error: Please enter a valid command!" << endl;
        }
    }
    catch (Invalid_Input &except) {
        cout << "Invalid input - " << except.msg_ptr << endl;
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

void NewObject(Model &model, char type, int id, int x, int y) {
    switch (type) {
        case 'c':
            cout << "Creating new center" << endl;
            if()
            model.AddCenter(id, Point2D(x, y));
            break;
        case 'g':
            cout << "Creating new gym" << endl;
            model.AddGym(id, Point2D(x, y));
            break;
        case 't':
            cout << "Creating new trainer" << endl;
            model.AddTrainer(id, Point2D(x, y));
            break;
        case 'w':
            cout << "Creating new wild pokemon" << endl;
            model.AddWild(id, Point2D(x, y));
            break;
        default:
            cout << "Not a valid object type" << endl;
            break;
    }
}
