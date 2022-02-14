#include "problem.h"


action problem_definition::actions() 
{
	uint8_t node = ambient.data[current.y][current.x];
	uint8_t evaluated_node;
	state test_state;

	// The node directly above the current node hasn't been evaluated
	if ((node & VISITED_UP) == DISCRETE_BMP_PATH) 
	{
		// Get possible next node
		test_state = up(current);
		evaluated_node = ambient.data[test_state.y][test_state.x];

		// Evaluate if the move to the considered node is valid.
		// Where invalid cases are when the node has been visited and exhausted.
		// When the node is represents a labyrinth wall.
		// Or when the considered action was the last action taken
		// Tested respectively:
		if (evaluated_node < 0b1111 
			&& evaluated_node != DISCRETE_BMP_WALL
			&& up != last_action)
			return up;
	}

	// All other if statements follow the same logic
	// but test for all the other 3 different actions and return their respective action
	if ((node & VISITED_DOWN) == DISCRETE_BMP_PATH)
	{
		test_state = down(current);
		evaluated_node = ambient.data[test_state.y][test_state.x];

		if (evaluated_node < 0b1111
			&& evaluated_node != DISCRETE_BMP_WALL
			&& down != last_action)
			return down;
	}

	if ((node & VISITED_RIGHT) == DISCRETE_BMP_PATH)
	{
		test_state = right(current);
		evaluated_node = ambient.data[test_state.y][test_state.x];

		if (evaluated_node < 0b1111
			&& evaluated_node != DISCRETE_BMP_WALL
			&& right != last_action)
			return right;
	}

	if ((node & VISITED_LEFT) == DISCRETE_BMP_PATH)
	{
		test_state = left(current);
		evaluated_node = ambient.data[test_state.y][test_state.x];

		if (evaluated_node < 0b1111
			&& evaluated_node != DISCRETE_BMP_WALL
			&& left != last_action)
			return left;
	}

	// if no valid action is found, nullptr is returned
	return nullptr;
}

state problem_definition::results(state current, action a) 
{
	return a(current);
}

bool problem_definition::goalTest(state current) 
{
	return ambient.data[current.y][current.x] == DISCRETE_BMP_GOAL;
}

float problem_definition::stepCost(state current, action a, state next)
{
	return 1.0f;
}

state up(state current)
{
	state out = current;
	out.y += 1;
	return out;
}

state down(state current)
{
	state out = current;
	out.y -= 1;
	return out;
}

state right(state current)
{
	state out = current;
	out.x += 1;
	return out;
}

state left(state current)
{
	state out = current;
	out.x -= 1;
	return out;
}