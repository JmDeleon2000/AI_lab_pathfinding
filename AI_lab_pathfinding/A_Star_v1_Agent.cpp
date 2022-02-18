#include "problem.h"
#include "math.h"
#define A_star_v1_debug 0
#include <iostream>

state Interest_points[2];

// used to represent 2d vector
float SGA[2], SGB[2];

// normalizes input vector
void normalize(float a[2]) 
{
	float mag;
	mag = sqrtf(a[0] * a[0] + a[1] * a[1]);
	 a[0] /= mag;
	 a[1] /= mag;
}

float dot_prod(float a[2], float b[2]) 
{
	return a[0] * b[0] + a[1] * b[1];
}

//  allows polymorphic behaviour for the two functions defined bellow
typedef bool (*visit_comparator)(const node_visit& a, const node_visit& b);

// function used by std::sort to compare visits when one goal has been found
// computes and compares the angle between the director vectors that describe the node visits and the desired path
bool One_goal_test(const node_visit& a, const node_visit& b)
{
	float SNA[2], SNB[2];
	float alpha, theta;
	SNA[0] = a.curr.x - a.last.x;
	SNA[1] = a.curr.y - a.last.y;
	normalize(SNA);
	alpha = dot_prod(SNA, SGA);
	alpha = alpha < 0 ? 2 : alpha;

	SNB[0] = b.curr.x - b.last.x;
	SNB[1] = b.curr.y - b.last.y;
	normalize(SNB);
	theta = dot_prod(SNB, SGA);
	theta = theta < 0 ? 2 : theta;

	return alpha < theta;
}

// function used by std::sort to compare visits when two goals have been found
// computes and compares the angle between the director vectors that describe the node visits and the desired path
bool Two_goal_test(const node_visit& a, const node_visit& b)
{
	float SNA[2], SNB[2];
	float alpha, beta, theta, gamma;
	SNA[0] = a.curr.x - a.last.x;
	SNA[1] = a.curr.y - a.last.y;
	normalize(SNA);
	alpha = dot_prod(SNA, SGA);
	alpha = alpha < 0 ? 2 : alpha;
	beta = dot_prod(SNA, SGB);
	beta = beta < 0 ? 2 : beta;

	SNB[0] = b.curr.x - b.last.x;
	SNB[1] = b.curr.y - b.last.y;
	normalize(SNB);
	theta = dot_prod(SNB, SGA);
	theta = theta < 0 ? 2 : theta;
	gamma = dot_prod(SNB, SGB);
	gamma = gamma < 0 ? 2 : gamma;

	return (alpha < theta && alpha < gamma) || (beta < theta && beta < gamma);
}

// vector angle A*
A_Star_v1_Agent::A_Star_v1_Agent(Discrete_image Ambient)
{
	ambient = Ambient;
	bool found_interest_points[3] = { false, false, false};
	//								  START, goal 

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
					&& (Interest_points[0].x - i) * (Interest_points[0].x - i) > 5
					&& (Interest_points[0].y - j) * (Interest_points[0].y - j) > 5)
				{
					Interest_points[1].y = j;
					Interest_points[1].x = i;
					found_interest_points[2] = true;
					if (found_interest_points[0]) 
					{
						j = ambient.height;
						break;
					}
				}
				else
				{
					Interest_points[0].y = j;
					Interest_points[0].x = i;
					found_interest_points[1] = true;
				}
			}
			i++;
		}
		j++;
	}

	SGA[0] = Interest_points[0].x - current.x;
	SGA[1] = Interest_points[0].y - current.y;
	normalize(SGA);
	if (found_interest_points[2]) 
	{
		SGB[0] = Interest_points[1].x - current.x;
		SGB[1] = Interest_points[1].y - current.y;
		normalize(SGB);
	}


	// selects comparasion function depending the amount of goals identified
	visit_comparator My_test = found_interest_points[2] ? Two_goal_test : One_goal_test;
	std::cout << "used: " << (found_interest_points[2] ? "Two_goal_test" : "One_goal_test") << "\n";

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
#if A_star_v1_debug
		std::cout << (int)current.x << " " << (int)current.y << "\n";
#endif

		if (goalTest(current))
			break;

		considered_visit.last = current;
		while (a = actions())
		{
			considered_state = results(current, a);
			considered_visit.curr = considered_state;
			considered_visit.total_cost = prev_cost + stepCost(current, a, considered_state);
			path.push_back(considered_visit);
		}
		map.insert(std::make_pair(current, current_visit));
		std::sort(path.begin(), path.end(), My_test);
	}
#if A_star_v1_debug
	std::cout << "\n\n";
	std::cout << (int)current.x << " " << (int)current.y << "\n";
	std::cout << (int)current_visit.last.x << " " << (int)current_visit.last.y << "\n";
	std::cout << "\n\n";
#endif


	// retrieve and mark the path from the map onto the discrete image for display
	auto search = map.find(current_visit.last);
	while (search->second.curr != search->second.last && search != map.end())
	{
#if A_star_v1_debug
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