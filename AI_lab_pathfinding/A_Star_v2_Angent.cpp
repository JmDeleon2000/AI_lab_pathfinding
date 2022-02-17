#include "problem.h"
#define A_star_v2_debug 0

#include <iostream>


state A_v2_star_goal[2];



// euclidean distance function
bool squaredEuclideanDistance(const node_visit& a, const node_visit& b) {
    float ax, ay, bx, by;
    
    ax = (a.curr.x - A_v2_star_goal[0].x);
    ay = (a.curr.y - A_v2_star_goal[0].y);
    bx = (b.curr.x - A_v2_star_goal[0].x);
    by = (b.curr.y - A_v2_star_goal[0].y);
    ax *= ax;
    ay *= ay;
    bx *= bx;
    by *= by;
    
    
    return ((ax + ay) < (bx + by));
}

// euclidean distance 
A_Star_v2_Agent::A_Star_v2_Agent(Discrete_image Ambient)
{
    ambient = Ambient;
    bool found_interest_points[3] = { false, false, false};
    //                                  START, goal

    //search for the start and the goals
    int i, j = 0;
    while (j < ambient.height)
    {
        i = 0;
        while (i < ambient.width)
        {
            if (ambient.data[j][i] == DISCRETE_BMP_START)
            {
                // set current state to match the starting node
                current.y = j;
                current.x = i;
                found_interest_points[0] = true;
                if (found_interest_points[0]
                    && found_interest_points[1]
                    && found_interest_points[2])
                {
                    j = ambient.height;
                    break;
                }
            }
            if (ambient.data[j][i] == DISCRETE_BMP_GOAL)
            {
                if (found_interest_points[1]
                    && (A_v2_star_goal[0].x - i) * (A_v2_star_goal[0].x - i) > 5
                    && (A_v2_star_goal[0].y - j) * (A_v2_star_goal[0].y - j) > 5)
                {
                    A_v2_star_goal[1].y = j;
                    A_v2_star_goal[1].x = i;
                    found_interest_points[2] = true;
                    if (found_interest_points[0])
                    {
                        j = ambient.height;
                        break;
                    }
                }
                else
                {
                    A_v2_star_goal[0].y = j;
                    A_v2_star_goal[0].x = i;
                    found_interest_points[1] = true;
                }
            }
            i++;
        }
        j++;
    }

   
    

    node_visit current_visit;
    node_visit considered_visit;
    current_visit.total_cost = 0;
    current_visit.curr = current;
    current_visit.last = current;
    state considered_state;
    action a;
    int prev_cost = 0;

    map.insert(std::make_pair(current, current_visit));
    path.push_back(current_visit);


    while (!path.empty())
    {
        current_visit = path.front();
        path.erase(path.begin());
        current = current_visit.curr;
        prev_cost = current_visit.total_cost;
#if A_star_v2_debug
        std::cout << (int)current.x << " " << (int)current.y << "\n";
#endif

        if (goalTest(current))
            break;

        considered_visit.last = current;
        while ((a = actions()))
        {
            considered_state = results(current, a);
            considered_visit.curr = considered_state;
            considered_visit.total_cost = prev_cost + stepCost(current, a, considered_state);
            path.push_back(considered_visit);
        }
        map.insert(std::make_pair(current, current_visit));
        std::sort(path.begin(), path.end(), squaredEuclideanDistance); // NO ESSTOY MUY SEGURA DE ESTO owo
    }
#if A_star_v2_debug
    std::cout << "\n\n";
    std::cout << (int)current.x << " " << (int)current.y << "\n";
    std::cout << (int)current_visit.last.x << " " << (int)current_visit.last.y << "\n";
    std::cout << "\n\n";
#endif


    // retrieve and mark the path from the map onto the discrete image for display
    auto search = map.find(current_visit.last);
    while (search->second.curr != search->second.last && search != map.end())
    {
#if A_star_v2_debug
        std::cout << (int)search->first.x << " " << (int)search->first.y << " ";
        std::cout << (int)search->second.last.x << " " << (int)search->second.last.y << " ";
        std::cout << (int)search->second.curr.x << " " << (int)search->second.curr.y << "\n";
        std::cout << search->second.total_cost << "\n";
#endif
        ambient.data[search->second.curr.y][search->second.curr.x] = DISCRETE_BMP_AWNSER;
        search = map.find(search->second.last);
    }
    ambient.data[search->second.curr.y][search->second.curr.x] = DISCRETE_BMP_AWNSER;
}
