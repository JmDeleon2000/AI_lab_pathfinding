#include "problem.h"
#include <iostream>

BFS_Agent :: BFS_Agent(Discrete_image Ambient)
{
	ambient = Ambient;

	//search for the start
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
				//ambient.data[j][i] = ambient.data[j][i] | 0x0f;
				j = ambient.height;
				break;
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
	path.push(current_visit);



	while (!path.empty())
	{
		current_visit = path.front();
		path.pop();
		current = current_visit.curr;
		prev_cost = current_visit.total_cost;
		std::cout << (int)current.x << " " << (int)current.y << "\n";

		if (goalTest(current))
			break;
		
		considered_visit.last = current;
		while (a = actions())
		{
			considered_state = results(current, a);
			considered_visit.curr = considered_state;
			considered_visit.total_cost = prev_cost + stepCost(current, a, considered_state);
			path.push(considered_visit);
		}
		map.insert(std::make_pair(current, current_visit));
	}
	std::cout << "\n\n\n\n";
	std::cout << (int)current.x << " " << (int)current.y << "\n";


	// retrieve and mark the path from the map
	auto search = map.find(current_visit.last);
	while (search->second.curr != search->second.last)
	{
		std::cout << (int)search->first.x << " " << (int)search->first.y << " ";
		std::cout << (int)search->second.last.x << " " << (int)search->second.last.y << " ";
		std::cout << (int)search->second.curr.x << " " << (int)search->second.curr.y << "\n";
		ambient.data[search->second.curr.y][search->second.curr.x] = DISCRETE_BMP_AWNSER;
		search = map.find(search->second.last);
	}
	ambient.data[search->second.curr.y][search->second.curr.x] = DISCRETE_BMP_AWNSER;
}