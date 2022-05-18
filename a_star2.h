#ifndef NPUZZLE_ASTARSEARCH_H
# define NPUZZLE_ASTARSEARCH_H

#define	MANHATTAN_DISTANCE 1
#define HAMMING_DISTANCE 2
#define LINEAR_CONFLICT 3

#include <set>
#include "node2.h"

#define ROW		5
#define	COLUMN	6
#define NUM		30

#define LIMIT_DEPTH 60
#define NODE_LIMIT 10000000

#define cost_ cost
#define parent_ parent

typedef int cost_t;
typedef int parent_t;
using pin = pair<int, Node>;

struct NodeInfo
{
	bool		isClosed; // is Closed
	cost_t		cost; // now Cost
	parent_t	parent; // parent

	bool operator==(const NodeInfo &other) const {
		return (parent == other.parent && cost == other.cost);
	}

	bool operator!=(const NodeInfo &other) const {
		return (!(other == *this));
	}
};

class AStarSearch
{
public:
	map<Node, NodeInfo>	visited;
	size_t				opendCount;
	int					maxDepth;
	int					nPushed;
	int					heuristicType;
	
	bool is_valid(int x, int y)
	{
		return (x >= 0 && y >= 0 && x < ROW && y < COLUMN);
	}

	static int	get_hamming_distance(const Node &a, const Node &b)
	{
		int	i;
		int	j;
		int	conflict;

		conflict = 0;
		for (i = 0; i < ROW; i++)
			for (j = 0; j < COLUMN; j++)
				// 값이 있으면서 서로 다르면 conflict
				if (a.board[i][j] != 0 && a.board[i][j] != b.board[i][j])
					conflict++;
		return (conflict);
	}
	// TODO mdTable 만드는 것은 기존에 방식이 더 빠를수도 있겠다.
	static int	get_manhattan_distance(const Node &a, const Node &b)
	{
		int			r, c;
		int			num;
		int			md;
		puzzle_t	pR[NUM]; // 임의의 값 num이 가지는 row index 저장할 예정
		puzzle_t	pC[NUM];

		md = 0;
		for (r = 0; r < ROW; r++)
		{
			for (c = 0; c < COLUMN; c++)
			{
				// num에 해당하는 row, col을 pR, pC에 각각 넣어 줌
				num = a.board[r][c];
				pR[num] = static_cast<puzzle_t>(r);
				pC[num] = static_cast<puzzle_t>(c);
			}
		}

		for (r = 0; r < ROW; r++)
		{
			for (c = 0; c < COLUMN; c++)
			{
				num = b.board[r][c];
				// 원하는 값과 얼마나 다른지 파악
				if (num != 0) 
					md += abs(pR[num] - r) // row의 차이
							+ abs(pC[num] - c); // column의 차이
			}
		}
		return (md);
	}

	static int	get_row_conflicts(const Node &a, const Node &b, puzzle_t pR[], puzzle_t pC[])
	{
		int	r, c;
		int	x, y;
		int	num1, num2;
		int	conflict;
		
		conflict = 0;
		// row 0부터 ROW - 1
		for (r = 0; r < ROW; r++)
		{
			// 한 줄마다 row conflict를 확인한다.
			// 0 인덱스부터 1 ~ COL - 1을 비교한다.
			for (x = 0; x < COLUMN; x ++)
			{
				num1 = b.board[r][x];
				for (y = x + 1; y < COLUMN; y++)
				{
					num2 = b.board[r][y];
					// 0이 아니면서
					if (num1 != 0 && num2 != 0)
					{
						// 모두 같은 로우에 있으면서
						if (r == pR[num1] && pR[num1] == pR[num2])
						{
							// num1(왼쪽에 있는 값이 더 커버리면 충돌 상태)
							if (pC[num1] > pC[num2])
								conflict++;
						}
					}
				}
			}
		}
		return (conflict);
	}

	static int	get_column_conflicts(const Node &a, const Node &b, puzzle_t pR[], puzzle_t pC[])
	{
		int	r, c;
		int	x, y;
		int	num1, num2;
		int	conflict;
		
		conflict = 0;
		// col 0 부터 COL - 1
		for (r = 0; r < COLUMN; r++)
		{
			// 한 줄마다 column conflict를 확인한다.
			// 0 인덱스부터 1 ~ ROW - 1을 비교한다.
			for (x = 0; x < ROW; x ++)
			{
				num1 = b.board[x][r];
				for (y = x + 1; y < ROW; y++)
				{
					num2 = b.board[y][r];
					// 0이 아니면서
					if (num1 != 0 && num2 != 0)
					{
						// 모두 같은 컬럼에 있으면서
						if (c == pC[num1] && pC[num1] == pC[num2])
						{
							// num1(위쪽에 있는 값이 더 커버리면 충돌 상태)
							if (pR[num1] > pR[num2])
								conflict++;
						}
					}
				}
			}
		}
		return (conflict);
	}

	static int	get_conflict_cnt(const Node &a, const Node &b)
	{
		int			r, c;
		int			num;
		int			conflict;
		puzzle_t	pR[NUM];
		puzzle_t	pC[NUM];

		for (r = 0; r < ROW; r++)
		{
			for (c = 0; c < COLUMN; c++)
			{
				// num에 해당하는 Row, col을 각각 넣어줌
				num = a.board[r][c];
				pR[num] = static_cast<puzzle_t>(r);
				pC[num] = static_cast<puzzle_t>(c);
			}
		}
		// find conflicts
		conflict = 0;
		conflict = get_row_conflicts(a, b, pR, pC);
		conflict += get_column_conflicts(a, b, pR, pC);
		return (conflict);
	}

	// h = md + 2 * lc
	static	int	get_linear_conflict(const Node &a, const Node &b)
	{
		return (get_manhattan_distance(a, b)
					+ 2 * get_conflict_cnt(a, b));
	}

	int	get_heuristic(const Node &a, const Node &b)
	{
		if (heuristicType == HAMMING_DISTANCE)
			return (get_hamming_distance(a, b));
		if (heuristicType == MANHATTAN_DISTANCE)
			return (get_manhattan_distance(a, b));
		if (heuristicType == LINEAR_CONFLICT)
			return (get_linear_conflict(a, b));
		return (0);
	}

	int	get_AStar_search_value(const Node &start, const Node &goal)
	{
		priority_queue<pin, vector<pin>, greater<vector<pin>::value_type> > openList;
		int			nExpanded;
		int			x, y, nx, ny;
		int			dir, nextCost, priority;
		bool		isVisited;
		Node		currentNode, nextNode;

		nExpanded = 0;
		maxDepth = 0;
		nPushed = 0;
		visited[start] = { false, 0, EOF }; // false 0 eof ??
		openList.push({0, start});

		// openList가 비어있지 않을 동안에
		while (!openList.empty())
		{
			// 1개의 자식 노드를 확인한다.
			currentNode = openList.top().second;
			openList.pop();
			nExpanded++; 

			NodeInfo &uInfo = visited[currentNode];
			uInfo.isClosed = true; // ??

			maxDepth = max(maxDepth, visited[currentNode].cost); // ??

			// goal을 찾았을 때
			if (currentNode == goal)
				break ;
			// 자식 부모 트리가 너무 깊어질 때
			if (uInfo.cost > LIMIT_DEPTH)
			{
				cout << "너무 많이 검색했습니다(HEIGHT).\n";
				break ;
			}
			if (visited.size() > NODE_LIMIT)
			{
				cout << "너무 많이 검색했습니다(NODE).\n";
				break ;
			}

			// 실제로 움직이는 부분
			x = -1, y = -1; // zeroIndex
			Node::get_zero_pos(currentNode, x, y);

			for (dir = 0; dir < 4; dir++)
			{
				nx = x + dirX[dir];
				ny = y + dirY[dir];

				// 다음 zeroIndex가 유효한 움직임이었다면?
				if (is_valid(nx, ny))
				{
					nextNode = currentNode;
					// 기존 0 위치와 다음 0 위치를 바꾸어줌
					swap(nextNode.board[x][y], nextNode.board[nx][ny]);
					
					// 이미 방문 했었는지 찾아봄
					// 값을 찾으면 find는 end를 반환하지 않음
					isVisited = visited.find(nextNode) != visited.end();
					// 방문이 되어있거나, 닫힌 상태면 더이상 확인하지 않음
					if (isVisited && visited[nextNode].isClosed)
						continue ;
					nextCost = uInfo.cost + 1;
					// 방문을 하지 않았거나
					// 기존의 방문보다 비용이 더 적게들었으면 다시 작업한다.
					if (!isVisited || nextCost < visited[nextNode].cost)
					{
						nPushed++;
						visited[nextNode] = {false, static_cast<cost_t>(nextCost), Node::opposite_direction(dir)};
						priority = nextCost + get_heuristic(nextNode, goal);
						openList.push({ priority, nextNode });
					}
				}
			}
		}
		opendCount = visited.size();
		return (nExpanded);
	}

	void	setHeuristic(int heuristic = MANHATTAN_DISTANCE)
	{
		heuristicType = heuristic;
	}

	virtual ~AStarSearch()
	{
		heuristicType = 0;
		visited.clear();
	}
};

#endif //NPUZZLE_ASTARSEARCH_H
