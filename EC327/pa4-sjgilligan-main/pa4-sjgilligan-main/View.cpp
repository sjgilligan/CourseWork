//
// Created by Sebbie on 11/22/2022.
//

#include "View.h"

bool View::GetSubscripts(int &out_x, int &out_y, Point2D location) {
    out_x = (location.x - origin.y) / scale;
    out_y = (location.y - origin.y) / scale;
    if (out_x <= view_maxsize && out_y <= view_maxsize) {
        return true;
    } else {
        cout << "An object is outside the display" << endl;
        return false;
    }
}

View::View() {
    size = 11;
    scale = 2;
    origin = Point2D(0, 0);
}

void View::Plot(GameObject *ptr) {
    int x;
    int y;
    if (ptr->ShouldBeVisible() && GetSubscripts(x, y, ptr->GetLocation()) && grid[x][y][0] == '.') {
        ptr->DrawSelf(grid[x][y]);
    } else if (GetSubscripts(x, y, ptr->GetLocation()) && *ptr->GetDisplayCode() != 'W') {
        grid[x][y][0] = '*';
        grid[x][y][1] = ' ';
    }
    //char *charptr = ptr->GetDisplayCode();

    //However, if there is already an
    //object plotted in that cell of the grid, the characters are replaced with ‘*’ and ' ' to
    //indicate that there is more than one object in that cell of the grid
}

void View::Clear() {
    for (int j = size - 1; j >= -1; j--) {
        for (int i = -1; i <= size - 1; i++) {
            //grid axis
            if (i == -1 && j % 2 == 0) {
                cout << j * 2;
                if (j / 5 == 0) {
                    cout << " ";
                }
            } else if (i == -1 && j % 2 != 0) {
                cout << "  ";
            } else if (j == -1 && i % 2 == 0) {
                cout << i * 2;
                if (i / 5 == 0) {
                    cout << " ";
                }
                cout << "  ";
            }
            //draw no objects
            if (i >= 0 && j >= 0) {
                grid[i][j][0] = '.';
                grid[i][j][1] = ' ';
                cout << grid[i][j][0] << grid[i][j][1];
            }
        }
        cout << endl;
    }
}


void View::Draw() {
    for (int j = size - 1; j >= -1; j--) {
        for (int i = -1; i <= size - 1; i++) {
            //grid axis
            if (i == -1 && j % 2 == 0) {
                cout << j * 2;
                if (j / 5 == 0) {
                    cout << " ";
                }
            } else if (i == -1 && j % 2 != 0) {
                cout << "  ";
            } else if (j == -1 && i % 2 == 0) {
                cout << i * 2;
                if (i / 5 == 0) {
                    cout << "  ";
                }
                cout << " ";
            }
            //draw objects
            if (i >= 0 && j >= 0) {
                cout << grid[i][j][0] << grid[i][j][1];
            }
        }
        cout << endl;
    }
}