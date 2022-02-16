#pragma once
#include "BMP.h"
#include <queue>
#include <stack>
#include <vector>
#include <unordered_map>


struct state
{
	uint8_t x;
	uint8_t y;
	bool operator == (state const& a) const;
};

bool operator != (state const& a, state const& b);
bool operator < (state const& a, state const& b);

class StateHash 
{
public:
	size_t operator()(const state& a)const;
};

// defines actions a functions that recieve a state and return another state
typedef state (*action)(state);

// possible actions:
state up(state s);
state down(state s);
state right(state s);
state left(state s);


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
	std::unordered_map<state, node_visit, StateHash> map;

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
	std::vector<node_visit> path;

public:
	A_Star_v1_Agent(Discrete_image Ambient);
};

class A_Star_v2_Agent : public  problem_definition
{
public:
	std::vector<node_visit> path;

public:
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


	
