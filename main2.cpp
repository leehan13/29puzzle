#include <chrono>
#include "node2.h"
#include "a_star2.h"

#define MAX 105

const std::string	RED("\033[0;31m");
const std::string	SKY("\033[0;36m");
const std::string	BLUE("\033[0;34m");
const std::string	RESET("\033[0m");

void	print_solution(AStarSearch &starSearch, const Node &start, const Node &goal)
{
	vector<char>	paths;
	vector<Node>	path;
	int				x, y;
	int				nx, ny;
	Node			now;

	now = goal;
	Node::get_zero_pos(now, x, y);
	while (starSearch.visited[now].parent_ != EOF)
	{
		path.push_back(now);
		now = now.getNode(starSearch.visited[now].parent_);
		Node::get_zero_pos(now, nx, ny);
		if (nx == x + 1 && ny == y)
			paths.push_back('U');
		else if (x == nx && ny == y + 1)
			paths.push_back('L');
		else if (nx == x - 1 && ny == y)
			paths.push_back('D');
		else
			paths.push_back('R');
		x = nx; 
		y = ny;
	}
	path.push_back(start);
	Node::get_zero_pos(start, x, y);
	reverse(path.begin(), path.end());
	reverse(paths.begin(), paths.end());
	for (auto &i : paths)
		cout << i << " ";
	cout << '\n';
}

void	ida_star(const Node &start, const Node &goal, int heuristic)
{
	AStarSearch	*starSearch;
	int			nExpanded;

	starSearch = new AStarSearch();
	starSearch->setHeuristic(heuristic);
	nExpanded = starSearch->get_AStar_search_value(start, goal);
	print_solution(*starSearch, start, goal);
	delete starSearch;
}

void	Solver(int puzzle[5][6])
{
	Node	start;
	Node	goal;

	for (int i = 0; i < ROW; i++)
		for (int j = 0; j < COLUMN; j++)
			goal.board[i][j] = static_cast<puzzle_t>(i * COLUMN + j + 1);

	goal.board[ROW - 1][COLUMN - 1] = 0;

	for (int i = 0; i < ROW; i++)
		for (int j = 0; j < COLUMN; j++)
			start.board[i][j] = static_cast<puzzle_t>(puzzle[i][j]);

	cout << "Start: \n" << start;
	cout << "Goal: \n" << goal;
	cout << "Manhattan Distance : " << AStarSearch::get_manhattan_distance(start, goal) << endl;
//	cout << "Hamming Distance : " << AStarSearch::get_hamming_distance(start, goal) << endl;
//	cout << "Linear Conflict : " << AStarSearch::get_linear_conflict(start, goal) << endl;

	cout << "# Linear Conflicts Heuristics: " << endl;
//	ida_star(start, goal, LINEAR_CONFLICT);

	cout << "# ManHattan Distance Heuristics: " << endl;
	ida_star(start, goal, MANHATTAN_DISTANCE);

	cout << "# Hamming Distance Heuristics: " << endl;
//	ida_star(start, goal, HAMMING_DISTANCE);
}

int	puzzle[5][6] = {
	{1, 8, 2, 3, 4, 6},
	{7, 20, 14, 9, 5, 12},
	{13, 21, 15, 10, 11, 18},
	{19, 22, 16, 0, 17, 24},
	{25, 26, 27, 28, 23, 29}
};

int puzzle2[5][6] = {
	{1, 20, 8, 2, 4, 12},
	{7, 21, 14, 3, 6, 18},
	{13, 22, 15, 11, 5, 24},
	{19, 0, 16, 9, 10, 17},
	{25, 26, 27, 28, 23, 29}
};

int	main(void)
{
	std::cout << SKY << "========  팀  ========\n";
	std::cout << RESET << "\t이상민\n\t이서영\n\t이찬웅\n";
	std::cout << SKY << "======================\n" << RESET;
	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
	Solver(puzzle2);
	std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
	std::chrono::milliseconds millisecond =
			std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	std::cout << "TimeCost: " << millisecond.count() << " ms\n";
}
