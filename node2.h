#ifndef NODE_H
# define NODE_H

# define	BOARD_SQ_SIZE 3
# define	PRINT_W 3

# define	ROW		5
# define	COLUMN	6
# define	NUM		30

# define	RIGHT 0
# define	LEFT 1
# define	DOWN 2
# define	UP 3

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>

typedef int direction_t;
typedef int8_t puzzle_t;

using namespace std;

int dirX[4] = {0, 0, 1, -1}; // RIGHT-LEFT-DOWN-UP
int dirY[4] = {1, -1, 0, 0}; // RIGHT-LEFT-DOWN-UP

class Node
{
public:
	puzzle_t	**board;
	bool		emptyNode;

	friend ostream &operator<<(ostream &os, const Node &node);
	// 매개변수 없이 초기화
	Node()
	{
		int	i;

		board = nullptr;
		emptyNode = true;
		// 2차원 배열 동적할당
		board = new puzzle_t *[ROW];
		for (i = 0; i < ROW; i++)
		{
			board[i] = new puzzle_t[COLUMN];
			// 할당 후 0으로 초기화
			memset(board[i], 0, COLUMN * sizeof(board[0][0]));
		}
	}

	// Node인자를 받아서 초기화
	Node(const Node &node)
	{
		int	i;
		int	j;

		this->~Node();
		this->emptyNode = node.emptyNode;
		board = new puzzle_t *[ROW];
		for (i = 0; i < ROW; i++)
		{
			board[i] = new puzzle_t[COLUMN];
		}
		for (i = 0; i < ROW; i++)
		{
			for (j = 0; j < COLUMN; j++)
			{
				board[i][j] = node.board[i][j];
			}
		}
	}

	Node &operator=(const Node &node)
	{
		int	i;
		int	j;

		this->~Node();
		this->emptyNode = node.emptyNode;
		board = new puzzle_t *[ROW];
		for (i = 0; i < ROW; i++)
		{
			board[i] = new puzzle_t[COLUMN];
		}
		for (i = 0; i < ROW; i++)
		{
			for (j = 0; j < COLUMN; j++)
			{
				board[i][j] = node.board[i][j];
			}
		}
		return *this;
	}

	~Node()
	{
		int	i;

		if (board == nullptr)
			return ;
		for (i = 0; i < ROW; i++)
//			delete board[i];
//		delete[] board;
		board = nullptr;
	}

	// board가 하나라도 다르면 False 리턴
	bool operator==(const Node &node) const {
		int	i;
		int	j;

		for (i = 0; i < ROW; i++)
		{
			for (j = 0; j < COLUMN; j++)
			{
				if (board[i][j] != node.board[i][j])
					return (false);
			}
		}
		return (true);
	}

	bool operator!=(const Node &node) const {
		return (!(*this == node));
	}

	// 좌측 상단부터 보드 값이 하나라도 크면 true
	bool operator<(const Node &node) const {
		int	i;
		int	j;

		for (i = 0; i < ROW; i++)
		{
			for (j = 0; j < COLUMN; j++)
			{
				if (board[i][j] < node.board[i][j])
					return (true);
				else if (board[i][j] > node.board[i][j])
					return (false);
				else
					continue ;
			}
		}
		return (false);
	}

	static bool	get_zero_pos(const Node &node, int &zx, int &zy)
	{
		int	i;
		int	j;

		zx = zy = -1;
		for (i = 0; i < ROW; i++)
		{
			for (j = 0; j < COLUMN; j++)
			{
				if (node.board[i][j] == 0)
				{
					zx = i;
					zy = j;
					return (true);
				}
			}
		}
		return (false);
	}

	static int	opposite_direction(int direction)
	{
		switch (direction)
		{
			case LEFT:
				return (RIGHT);
			case RIGHT:
				return (LEFT);
			case UP:
				return (DOWN);
			case DOWN:
				return (UP);
			default:
				return EOF;
		}
	}

	static int	get_inv_count(const vector<int> &arr)
	{
		int	i, j;
		int	cnt;

		cnt = 0;
		for (i = 0; i < arr.size() - 1; i++)
			for (j = i + 1; j < arr.size(); j++)
				if (arr[i] > arr[j])
					cnt++;
		return (cnt);
	}

	bool	is_empty_node() const {
		return (emptyNode);
	}

	Node	getNode(int direction, int zx = -1, int zy = -1)
	{
		Node	node;
		int		nx;
		int		ny;

		// direction 3 ?
		if (board == nullptr || direction > 3)
			return (*this);
		// invalid -> 새로운 노드 반환
		if (zx == -1 || zy == -1) {
			if (!get_zero_pos(*this, zx, zy))
				return (Node());
		}

		nx = zx + dirX[direction];
		ny = zy + dirY[direction];
		if (nx < 0 || ny < 0 || nx >= ROW || ny >= COLUMN)
			return (Node());

		node = *this;
		// 왜 바꾸어주는 것일까
		swap(node.board[zx][zy], node.board[nx][ny]);
		return (node);
	}
};

ostream &operator<<(ostream &os, const Node &node)
{
	int	i;
	int	j;

	if (!node.board)
		return (os);
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COLUMN; j++)
			if (node.board[i][j])
				os << setw(PRINT_W) << (static_cast<int>(node.board[i][j])) << " ";
			else
				os << setw(PRINT_W) << "  " << " ";
		os << endl;
	}
	os << " ----------- " << std::endl;
	return os;
}
#endif

