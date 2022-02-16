#include "problem.h"


action problem_definition::actions() 
{
	uint8_t node = ambient.data[current.y][current.x];
	uint8_t evaluated_node;
	state test_state;

	action action_list[4] = { up, down, right, left };
	//action inv_action_list[4] = { down, up, left, right };
	uint8_t mask_list[4] = { VISITED_UP, VISITED_DOWN, VISITED_RIGHT, VISITED_LEFT };

	int i = 0;
	while (i < 4)
	{
		// The node directly above the current node hasn't been evaluated
		if ((node & mask_list[i]) == DISCRETE_BMP_PATH)
		{
			// Get possible next node
			test_state = action_list[i](current);
			if (test_state.x >= DISCRETE_BOUND
				|| test_state.y >= DISCRETE_BOUND
				|| test_state.x < 0
				|| test_state.y < 0)
			{
				i++;
				continue;
			}
			evaluated_node = ambient.data[test_state.y][test_state.x];

			// Evaluate if the move to the considered node is valid.
			// Where invalid cases are when the node has been visited and exhausted.
			// When the node is represents a labyrinth wall.
			// Or when the considered action was the last action taken
			// Tested respectively:
			if ((evaluated_node == DISCRETE_BMP_PATH
				|| evaluated_node == DISCRETE_BMP_GOAL
				|| evaluated_node == DISCRETE_BMP_START)
				&& evaluated_node != DISCRETE_BMP_WALL)
			{
				ambient.data[current.y][current.x] = ambient.data[current.y][current.x] | mask_list[i];
				return action_list[i];
			}
		}
		i++;
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

bool operator != (state const &a, state const& b)
{
	return (a.x != b.x || a.y != b.y);
}

bool operator < (state const& a, state const& b)
{
	return ((int)a.x + (int)a.y < (int)b.x + (int)b.y);
}

// Manages hash map collision
bool state::operator == (state const& a) const
{
	return (a.x == x && a.y == y);
}

// defines state hashing behaviour
size_t StateHash::operator()(const state &a) const
{
	return ((int)a.x * (int)a.x) + DISCRETE_BOUND - a.y;
}