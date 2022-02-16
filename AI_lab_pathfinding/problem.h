#pragma once
#include "BMP.h"
#include <queue>
#include <stack>
#include <map>


struct state
{
	uint8_t x;
	uint8_t y;
};

bool operator != (state const& a, state const& b);
bool operator < (state const& a, state const& b);

// defines actions a functions that recieve a state and return another state
typedef state (*action)(state);

// possible actions:
state up(state s);
state down(state s);
state right(state s);
state left(state s);


// void valid_actions;
// valid_actions = actions();
// int i = 0;
// bool was_valid = false;
// while (i < 4) 
//{		
//		switch(valid_actions.mask[i])
//		{
//			case UP_VALID (o cualquiera):
//			was_valid = true;
//			matriz[current.y][current.x] = matriz[current.y][current.x] | VISITED_UP;
//			// depende de qué algoritmo es pero:
//			// lo escribe en la estructura path
//			current = up(current);
//			i = 4;
//			break;
//		}
//		i++;
//}
//if (was_valid)
//{
//		continuar con el siguiente nodo;
//}
//else 
//{
//		backtracking;
//}

// used to trace back the resultant path
struct node_visit
{
	state curr;
	state last;
	int total_cost; //siempre se le suma 1 porque todos los nodos son iguales
};


// class that defines the interface and implements utility functions
class problem_definition {

public:
	state current;
	Discrete_image ambient;
	action last_action;
	std::map<state, node_visit> map;

public:

	// returns the next action to execute based on step costs and heuristics (if any)
	// returns null if there are no valid actions for the current node
	action actions();
	
	// returns a new state
	state results(state current, action a);

	// returns True or False if the goal has already been reached
	bool goalTest(state current);

	// returns a real number trivially 1.0f
	float stepCost(state current, action a , state next);

	// returns a real number that determines the best possible solution
	//virtual float pathCost() = 0;
};

	
class BFS_Agent : public problem_definition
{
public:
	std::queue<node_visit> path;
	
public:
	BFS_Agent(Discrete_image Ambient);
};


class DFS_Agent : public  problem_definition
{
public:
	std::stack<node_visit> path;
	Discrete_image ambient;

public:
	DFS_Agent(Discrete_image Ambient);
};



class A_Star_v1_Agent : public  problem_definition
{
public:
	std::queue<node_visit> path;

public:
	action actions();
	A_Star_v1_Agent(Discrete_image Ambient);
};

class A_Star_v2_Agent : public  problem_definition
{
public:
	std::queue<node_visit> path;

public:
	action actions();
	A_Star_v2_Agent(Discrete_image Ambient);
};


































// #include <iostream>

// using namespace std;

// class problem_definition {
// public:
// 	virtual void state() = 0;
// };

// class actions: public problem_definition {
// public:
// 	void state();
// };

// class results: public problem_definition {
// public:
// 	void state();
// };

// class goalTest: public problem_definition {
// public:
// 	void state();
// };

// class stepCost: public problem_definition {
// public:
// 	void state();
// };

// class pathCost: public problem_definition {
// public:
// 	void state();
// };


	
