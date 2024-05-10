#include "Map.h"
#include <vector>
#include <iostream>
#include <Windows.h>


class SnakeHead {
public:
	int x;
	int y;
	char head = '@';


	SnakeHead(int startX, int startY) : x(startX), y(startY) {}

	void move(int dx, int dy) {
		x += dx;
		y += dy;
	}

	void teleportMove(int dx, int dy) {
		x = dx;
		y = dy;
	}
};

class SnakeBody {
public:
	int x;
	int y;
	char body = 'O';

	SnakeBody() : x(0), y(0) {}
	SnakeBody(int startX, int startY) : x(startX), y(startY) {}

	void move(int dx, int dy) {
		x = dx;
		y = dy;
	}
};


class Snake {
public:
	enum snakeDirection {
		LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3
	};

	// 맨 처음은 오른쪽을 보는 상태로 시작..
	snakeDirection currDir = RIGHT;
	SnakeHead head;
	std::vector<SnakeBody> body;
	bool isDie = false;
	int tempTailX, tempTailY;

	int snakeSize;

	Snake(int startX, int startY) : head(startX, startY) {
		// 뱀의 맨 처음 크기
		snakeSize = 3;

		// 벡터 크기 3으로 시작..
		body.resize(2);

		// 뱀은 맨 처음 시작할 때 오른쪽을 보고 있을 것이므로
		// x 좌표 위치를 -i 해준다..
		body[0] = { startX - 1, startY };
		body[1] = { startX - 2, startY };


		tempTailX = body.back().x;
		tempTailY = body.back().y;
	}


	int moveX = 0;
	int moveY = 0;
	int gateCount = 0;


public:
	void ReSetSnake() {
		snakeSize = 3;
		gateCount = 0;
		body.resize(2);

		tempTailX = body.back().x;
		tempTailY = body.back().y;
	}

	void gotoxy(int x, int y) {
		COORD pos = { x, y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	}

	void draw() {
		gotoxy(head.x, head.y);
		std::cout << head.head;

		for (int i = 0; i < snakeSize - 1; i++) {
			gotoxy(body[i].x, body[i].y);
			std::cout << body[i].body;
		}
	}

	void removeTail() {
		gotoxy(tempTailX, tempTailY);
		std::cout << ' ';
	}

	void removeTail(int x, int y) {
		gotoxy(x, y);
		std::cout << ' ';
	}

	void setPos(snakeDirection dir) {
		switch (dir) {
		case LEFT:
			if (currDir == RIGHT) {
				isDie = true;
				return;
			}
			else {
				moveX = -1;
				moveY = 0;
			}
			break;

		case RIGHT:
			if (currDir == LEFT) {
				isDie = true;
				return;
			}
			else {
				moveX = 1;
				moveY = 0;
			}
			break;

		case UP:
			if (currDir == DOWN) {
				isDie = true;
				return;
			}
			else {
				moveX = 0;
				moveY = -1;
			}
			break;

		case DOWN:
			if (currDir == UP) {
				isDie = true;
				return;
			}
			else {
				moveX = 0;
				moveY = 1;
			}
			break;
		}

		currDir = dir;
	}

	void move(int x, int y) {
		tempTailX = body.back().x;
		tempTailY = body.back().y;


		// head 먼저 움직이기..
		int prevX = head.x;
		int prevY = head.y;
		head.move(x, y);

		for (int i = 0; i < snakeSize - 1; i++) {
			int tempX = body[i].x;
			int tempY = body[i].y;

			body[i].move(prevX, prevY);
			prevX = tempX;
			prevY = tempY;
		}

		removeTail();
		draw();

	}

	// 이 함수는 뱀 머리랑 게이트가 만나면 실행될 것..
	void teleportMove(int x, int y) {
		tempTailX = body.back().x;
		tempTailY = body.back().y;

		// head 먼저 움직이기..
		int prevX = head.x;
		int prevY = head.y;
		head.teleportMove(x, y);

		for (int i = 0; i < snakeSize - 1; i++) {
			int tempX = body[i].x;
			int tempY = body[i].y;

			body[i].move(prevX, prevY);
			prevX = tempX;
			prevY = tempY;
		}

		removeTail();
		draw();
	}

	bool snakeTouchWall() {
		if (head.x <= 0 || head.x >= Map::MAPWIDTH || head.y <= 0 || head.y >= Map::MAPHEIGHT) {
			isDie = true;
			return true;
		}
		return false;
	}

	void snakeTouchFreshApple() {
		snakeSize += 1;

		// 꼬리쪽이 늘어나야 하므로 현재 이동 방향과 반대되는 곳으로 늘어나기
		// 예를 들면, 현재 이동 방향이 LEFT 로 moveX = -1, moveY = 0 일 때, -moveX 해서 +1, -moveY 해서 0
		// 이걸 맨 마지막 바디의 x, y 에 더해주면 맨 마지막에 꼬리가 하나 붙음..
		body.emplace_back(body.back().x - moveX, body.back().y - moveY);
	}

	void snakeTouchPoisonApple() {
		snakeSize -= 1;

		// 맨 마지막 꼬리의 위치를 기억해놓고
		SnakeBody temp = body.back();
		// 맨 마지막 꼬리의 위치로 가서 꼬리를 지우고
		removeTail(temp.x, temp.y);
		// 맨 마지막 꼬리를 잘라냄..
		body.pop_back();

		//Sleep(5);
	}


	// gate1 은 뱀이 들어가는 게이트, gate2 는 뱀이 나가는 게이트, snakeMoveX 랑 snakeMoveY 는 현재 뱀이 이동하는 방향
	void snakeTouchGate(Gate* gate1, Gate* gate2, int snakeMoveX, int snakeMoveY, Snake* snake) {
		gateCount += 1;
		int tempGate2PosX = (*gate2).gatePosX;
		int tempGate2PosY = (*gate2).gatePosY;

		// 이 isSnakeMoved 변수가 true 일 때에는 게이트 위치가 새로 업데이트 되면 안됨..
		(*gate1).isSnakeMoved = true;
		(*gate2).isSnakeMoved = true;

		// 뱀이 들어가는 게이트를 기준으로 생각했을 때, 뱀이 어느쪽으로 들어오는지는 현재 뱀의 이동방향의 반대임..
		// 즉, 현재 뱀의 이동 방향이 LEFT 라면, currSnakeInputDir 는 그의 반대인 RIGHT..
		if (snakeMoveX == -1 && snakeMoveY == 0)
			(*gate1).currSnakeInputDir = Gate::RIGHT;
		else if (snakeMoveX == 1 && snakeMoveY == 0)
			(*gate1).currSnakeInputDir = Gate::LEFT;
		else if (snakeMoveX == 0 && snakeMoveY == -1)
			(*gate1).currSnakeInputDir = Gate::DOWN;
		else if (snakeMoveX == 0 && snakeMoveY == 1)
			(*gate1).currSnakeInputDir = Gate::UP;


		// 뱀이 gate1 으로 들어갔으므로.. gate2 로 나와야함..
		// 그래서 계획으로는 snakeHead 클래스에 teleportMove 라는 함수를 만들어서 그쪽으로 아예 위치를 옮기도록 해야할 듯!
		// snake 클래스에도 teleportMove 함수 따로 만들어서 호출할 것..
		// 뱀의 머리가 어디로 이동해야 하나면.. gate2 의 위치 바로 전 위치.. (만약 gate2 가 하단 가장자리에 있으면 그 위치 바로 위, 좌측 가장자리면 그 위치 바로 오른쪽)
		if ((*gate2).isEdgeGate) {
			if ((*gate2).gatePosX == 0) {
				// 뱀이 나가는 게이트가 좌측 가장자리에 있으면 뱀의 이동방향은 RIGHT 로 바꿔줘야함..
				(*snake).setPos(Snake::RIGHT);
				(*snake).teleportMove(1, (*gate2).gatePosY);

				tempGate2PosX += 1;
			}
			else if ((*gate2).gatePosX == 45) {
				// 뱀이 나가는 게이트가 우측 가장자리에 있으면 뱀의 이동방향은 LEFT 로 바꿔줘야함..
				(*snake).setPos(Snake::LEFT);
				(*snake).teleportMove(44, (*gate2).gatePosY);

				tempGate2PosX -= 1;
			}
			else if ((*gate2).gatePosY == 0) {
				// 뱀이 나가는 게이트가 상단 가장자리에 있으면 뱀의 이동방향은 DOWN 으로 바꿔줘야함..
				(*snake).setPos(Snake::DOWN);
				(*snake).teleportMove((*gate2).gatePosX, 1);

				tempGate2PosY += 1;
			}
			else if ((*gate2).gatePosY == 22) {
				// 뱀이 나가는 게이트가 하단 가장자리에 있으면 뱀의 이동방향은 UP 으로 바꿔줘야함..
				(*snake).setPos(Snake::UP);
				(*snake).teleportMove((*gate2).gatePosX, 21);

				tempGate2PosY -= 1;
			}
		}


		while (true) {
			// 뱀의 꼬리가 나가는 게이트 바로 직전 위치에 도달할 때까지 순환문 돌도록..
			if ((*snake).body.back().x == tempGate2PosX && (*snake).body.back().y == tempGate2PosY) {
				(*gate1).isSnakeMoved = false;
				(*gate2).isSnakeMoved = false;
				break;
			}

			(*snake).move((*snake).moveX, (*snake).moveY);
			Sleep(100);
		}
	}
};