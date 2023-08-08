#include "Building.h"

Building::Building() {
    trainer_count = 0;
    display_code = 'B';
    cout << "Building default constructed" << endl;
}

Building::Building(char in_code, int in_id, Point2D in_loc) {
    display_code = in_code;
    location = in_loc;
    id_num = in_id;
    cout << "Building constructed" << endl;
}

void Building::AddOneTrainer() {
    trainer_count++;
}

void Building::RemoveOneTrainer() {
    trainer_count--;
}

void Building::ShowStatus() {
    cout << display_code << id_num << " located at " << location << endl;
    if (trainer_count == 1) {
        cout << trainer_count << " trainer is in this building" << endl;
    } else {
        cout << trainer_count << " trainers are in this building" << endl;
    }
}

bool Building::ShouldBeVisible() {
    return true;
}
