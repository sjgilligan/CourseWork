#include "GameObject.h"
#include "Point2D.h"

GameObject::GameObject() {
    display_code = 'o';
    id_num = 0;
    state = 0;
}

GameObject::GameObject(char in_code) {
    display_code = in_code;
    id_num = 1;
    state = 0;
    cout << "GameObject constructed" << endl;
}

GameObject::GameObject(Point2D in_loc, int in_id, char in_code) {
    location = in_loc;
    display_code = in_code;
    id_num = in_id;
    state = 0;
    cout << "GameObject constructed" << endl;
}

GameObject::~GameObject() {
    cout << "GameObject destructed" << endl;
}

Point2D GameObject::GetLocation() {
    return this->location;
}

int GameObject::GetId() {
    return this->id_num;
}

char *GameObject::GetDisplayCode() {
    return &display_code;
}

char GameObject::GetState() {
    return state;
}

void GameObject::ShowStatus() {
    cout << display_code << id_num << " at " << location << endl;
}

void GameObject::DrawSelf(char *ptr) {
    *ptr = display_code;
    *(ptr + 1) = '0' + id_num;
}

bool GameObject::ShouldBeVisible() {
    
}