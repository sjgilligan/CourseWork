#include "escape_zoo.h"
#include "vector"
#include <iostream>
#include <math.h>

//https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/

using namespace std;

class CoordinateHeadingPair {
private:
    vector<CoordinateHeadingPair *> adjnodes;
public:
    coordinate *loc;
    Heading heading;
    bool escape;

    CoordinateHeadingPair(coordinate *l, Heading h, bool e) {
        loc = l;
        heading = h;
        escape = e;
    }
};


bool valid_move(coordinate *current, int vert_change, int horz_change, vector<coordinate *> nodes) {
    coordinate temp;
    temp.first = current->first + vert_change;
    temp.second = current->second + horz_change;
    for (int i = 0; i < nodes.size(); i++) {
        if (temp.first == nodes[i]->first && temp.second == nodes[i]->second) {
            return true;
        }
    }
    return false;
}

bool is_escape(coordinate *current, vector<coordinate *> escape_nodes) {
    for (int i = 0; i < escape_nodes.size(); i++) {
        if (current->first == escape_nodes[i]->first && current->second == escape_nodes[i]->second) {
            return true;
        }
    }
    return false;
}

int min_distance(int *distance,bool *used,int V){
    int min = 10000;
    int min_index;

    for(int i = 0; i < V; i++){
        if(used[i] == false && distance[i] <= min){
            min = distance[i];
            min_index = i;
        }
    }
    return min_index;
}

Action get_action(Heading current_heading, Heading previous_heading) {
    if (current_heading == previous_heading) {
        return go_straight;
    } else {
        return go_left;
    }
}


motion_plan escape_route(grid const &M, coordinate init_coordinate, Heading init_heading) {
    vector < coordinate * > nodes;
    int i;
    int j;
    for (i = 0; i < M.size(); i++) {
        for (j = 0; j < M[i].size(); j++) {
            if (M[i][j] == 0) {
                coordinate *c = new coordinate(i, j);
                nodes.push_back(c);
                //cout << "cord " << i << "," << j << " added" << endl;
            }
        }
    }

    vector < coordinate * > escape_nodes;
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes[i]->first == 0 || nodes[i]->first == (M.size() - 1) || nodes[i]->second == 0 ||
            nodes[i]->second == (M[0].size() - 1)) {
            coordinate *e = new coordinate(nodes[i]->first, nodes[i]->second);
            escape_nodes.push_back(e);
            //cout << "node " << nodes[i]->first << "," << nodes[i]->second << " is escape" << endl;
        }
    }

    vector < CoordinateHeadingPair * > CHPs;

    for (int i = 0; i < nodes.size(); i++) {
        //if(is_escape(nodes[i], escape_nodes)){
        CoordinateHeadingPair *c1 = new CoordinateHeadingPair(nodes[i], North, is_escape(nodes[i], escape_nodes));
        CHPs.push_back(c1);
        CoordinateHeadingPair *c2 = new CoordinateHeadingPair(nodes[i], South, is_escape(nodes[i], escape_nodes));
        CHPs.push_back(c2);
        CoordinateHeadingPair *c3 = new CoordinateHeadingPair(nodes[i], East, is_escape(nodes[i], escape_nodes));
        CHPs.push_back(c3);
        CoordinateHeadingPair *c4 = new CoordinateHeadingPair(nodes[i], West, is_escape(nodes[i], escape_nodes));
        CHPs.push_back(c4);
        /*}else {
            if (valid_move(nodes[i], -1, 0, nodes) || valid_move(nodes[i],0,-1,nodes)) {
                CoordinateHeadingPair *c1 = new CoordinateHeadingPair(nodes[i], North,
                                                                      is_escape(nodes[i], escape_nodes));
                CHPs.push_back(c1);
            }
            if (valid_move(nodes[i], 1, 0, nodes) || valid_move(nodes[i],0,1,nodes)) {
                CoordinateHeadingPair *c2 = new CoordinateHeadingPair(nodes[i], South,
                                                                      is_escape(nodes[i], escape_nodes));
                CHPs.push_back(c2);
            }
            if (valid_move(nodes[i], 0, -1, nodes) || valid_move(nodes[i],1,0,nodes)) {
                CoordinateHeadingPair *c3 = new CoordinateHeadingPair(nodes[i], East,
                                                                      is_escape(nodes[i], escape_nodes));
                CHPs.push_back(c3);
            }
            if (valid_move(nodes[i], 0, 1, nodes) || valid_move(nodes[i],-1,0,nodes)) {
                CoordinateHeadingPair *c4 = new CoordinateHeadingPair(nodes[i], West,
                                                                      is_escape(nodes[i], escape_nodes));
                CHPs.push_back(c4);
            }
        }*/
    }

    vector <vector<int>> adj_matrix(CHPs.size(), vector<int>(CHPs.size()));

    for (int i = 0; i < CHPs.size(); i++) {
        for (int j = 0; j < CHPs.size(); j++) {
            adj_matrix[i][j] = 0;
        }
    }


    for (int i = 0; i < CHPs.size(); i++) {
        for (int j = 0; j < CHPs.size(); j++) {
            switch (CHPs[i]->heading) {
                case (North): {
                    if (CHPs[j]->heading == North && (CHPs[j]->loc->first == (CHPs[i]->loc->first - 1)) && (CHPs[j]->loc->second == CHPs[i]->loc->second )) {
                        adj_matrix[i][j] = 1;
                    }
                    if (CHPs[j]->heading == West && (CHPs[j]->loc->second == (CHPs[i]->loc->second - 1)) && (CHPs[j]->loc->first == CHPs[i]->loc->first )){
                        //if (CHPs[j]->heading == East && (CHPs[j]->loc->first == (CHPs[i]->loc->first - 1)) && (CHPs[j]->loc->second == CHPs[i]->loc->second )){
                        adj_matrix[i][j] = 1;
                    }
                    break;
                }
                case (South): {
                    if (CHPs[j]->heading == South && (CHPs[j]->loc->first == (CHPs[i]->loc->first + 1)) && (CHPs[j]->loc->second == CHPs[i]->loc->second)) {
                        adj_matrix[i][j] = 1;
                    }
                    if (CHPs[j]->heading == East && (CHPs[j]->loc->second == (CHPs[i]->loc->second + 1)) && (CHPs[j]->loc->first == CHPs[i]->loc->first)) {
                        //if (CHPs[j]->heading == West && (CHPs[j]->loc->first == (CHPs[i]->loc->first + 1)) && (CHPs[j]->loc->second == CHPs[i]->loc->second)) {
                        adj_matrix[i][j] = 1;
                    }
                    break;
                }
                case (East): {
                    if (CHPs[j]->heading == East && (CHPs[j]->loc->second == (CHPs[i]->loc->second + 1)) && (CHPs[j]->loc->first == CHPs[i]->loc->first)) {
                        adj_matrix[i][j] = 1;
                    }
                    if (CHPs[j]->heading == North && (CHPs[j]->loc->first == (CHPs[i]->loc->first - 1)) && (CHPs[j]->loc->second == CHPs[i]->loc->second)) {
                        //if (CHPs[j]->heading == South && (CHPs[j]->loc->second == (CHPs[i]->loc->second - 1)) && (CHPs[j]->loc->first == CHPs[i]->loc->first)) {
                        adj_matrix[i][j] = 1;
                    }
                    break;
                }
                case (West): {
                    if (CHPs[j]->heading == West && (CHPs[j]->loc->second == (CHPs[i]->loc->second - 1)) && (CHPs[j]->loc->first == CHPs[i]->loc->first)) {
                        adj_matrix[i][j] = 1;
                    }
                    if (CHPs[j]->heading == South && (CHPs[j]->loc->first == (CHPs[i]->loc->first + 1)) && (CHPs[j]->loc->second == CHPs[i]->loc->second)) {
                        //if (CHPs[j]->heading == North && (CHPs[j]->loc->second == (CHPs[i]->loc->second + 1)) && (CHPs[j]->loc->first == CHPs[i]->loc->first)) {
                        adj_matrix[i][j] = 1;
                    }
                    break;
                }
            }
        }
    }

    /*for (int i = 0; i < CHPs.size(); i++) {
        cout << "(" << CHPs[i]->loc->first << "," << CHPs[i]->loc->second << ") " << CHPs[i]->heading << "  ";
        for (int j = 0; j < CHPs.size(); j++) {
            cout << adj_matrix[i][j] << " ";
        }
        cout << endl;
    }*/






    vector<motion_plan> all_plans(CHPs.size(), vector<Action>());
    motion_plan output;

    int distance[CHPs.size()];
    bool used[CHPs.size()];

    for(int i = 0; i < CHPs.size();i++){
        distance[i] = 10000;
        used[i] = false;
    }

    CoordinateHeadingPair Source(&init_coordinate,init_heading,0);

    for(int i = 0; i < CHPs.size();i++){
        if(CHPs[i]->loc->first == init_coordinate.first && CHPs[i]->loc->second == init_coordinate.second && CHPs[i]->heading == init_heading){
            distance[i] = 0;
        }
    }

    for(int i = 0; i < CHPs.size() - 1; i++) {
        //cout << "pass " << i << endl;
        int u = min_distance(distance, used, CHPs.size());

        used[u] = true;

        for(int j = 0; j < CHPs.size(); j++){
            if(!used[j] && adj_matrix[u][j] && distance[u] != 10000 && (distance[u] + adj_matrix[u][j] < distance[j])) {
                distance[j] = distance[u] + adj_matrix[u][j];
                //cout << CHPs[j]->heading << " " << CHPs[j]->loc->first << " " << CHPs[j]->loc->second << endl;
                all_plans[j] = all_plans[u];
                if(CHPs[j]->heading == CHPs[u]->heading){
                    all_plans[j].push_back(go_straight);
                }else{
                    all_plans[j].push_back(go_left);
                }
            }
        }
    }

    vector<int> possible_outputs;

    for(int i = 0; i < CHPs.size();i++){
        if(CHPs[i]->escape && distance[i] < 10000){
            //cout << CHPs[i]->heading <<" "<<CHPs[i]->loc->first<< " " << CHPs[i]->loc->second << " " << distance[i] << endl;
            possible_outputs.push_back(i);
        }
    }

    int min = 1000;
    int min_index;

    for(int i = 0;i < possible_outputs.size();i++){
        if(all_plans[possible_outputs[i]].size() < min){
            min = all_plans[possible_outputs[i]].size();
            min_index = possible_outputs[i];
        }
    }

    output = all_plans[min_index];

    //cout << min;

    /*for(int i = 0; i < all_plans.size();i++){
        if(all_plans[i].size() == min){
            output = all_plans[i];
        }
    }*/

    //output = all_plans[];
    return {output};
}