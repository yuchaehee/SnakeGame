#include <Windows.h>
#include <iostream>
#include <vector>
#include <ctime>

#include "GateEmergeEnablePos.h"
#include "Gate.h"
#include "Snake.cpp"
#include "Food.cpp"
#include "UserInfo.cpp"
#include <ctime>
#include "Map.h"



using namespace std;


void gotoxy(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


int main() {
    // 맵 생성
    Map mapInstance;
    mapInstance.setMap();


    // 아이템 생성 
    Food food;

    // 사용자 정보 생성
    UserInfo Info;

    // 아이템의 아이디 랜덤 변수
    random_device randomId;
    mt19937 genItemId(randomId());
    uniform_int_distribution<int> idDis(3, 4);

    // 아이템의 위치 랜덤 변수
    random_device randomPosX;
    random_device randomPosY;
    mt19937 genItemPosX(randomPosX());
    mt19937 genItemPosY(randomPosY());
    uniform_int_distribution<int> posXDis(2, 44);
    uniform_int_distribution<int> posYDis(2, 21);


    // 시간 관련 변수
    clock_t startTime = clock();
    clock_t tempTime{};

    clock_t itemEmergeTime{};
    clock_t gateEmergeTime{};


    // 지렁이 생성
    // 맨 처음 위치는 10, 10 으로..
    Snake snake(10, 10);


    // 게이트 생성
    Gate gate1(2, 0);
    Gate gate2(2, 22);
    //GateEmergeEnablePos prevGate1(2, 0);
    //GateEmergeEnablePos prevGate2(2, 23);

    gate1.setGateEmergeVec(mapInstance.map, mapInstance.mapLevel);
    gate2.setGateEmergeVec(mapInstance.map, mapInstance.mapLevel);

    bool gateMapStarted = false;

    // 게이트 위치 랜덤 변수
    random_device randomVecIndex;
    mt19937 genVecIndex(randomVecIndex());
    //uniform_int_distribution<int> vecIndexDis(0);


    while (true) {
        tempTime = clock();
        Info.setTime(difftime(tempTime, startTime) / 1000.0);

        // 초단위
        // 게임 시작한지 5초 지나면 maplevel이 2가 되도록..
        // 아이템 등장..(5초 간격으로..)
        if (mapInstance.mapLevel == 0 && difftime(tempTime, startTime) / 1000.0 > 5) {
            // 이제 맵레벨은 2가 됨..
            mapInstance.mapLevel++;
            mapInstance.setMap();
        }
        // 게임 시작한지 40초 지나면 maplevel이 3이 되도록..
        // 게이트 등장..
        if (mapInstance.mapLevel == 1 && difftime(tempTime, startTime) / 1000.0 > 10) {
            mapInstance.mapLevel++;
            mapInstance.setMap();
        }

        if ((mapInstance.mapLevel == 1 || mapInstance.mapLevel == 2) && difftime(tempTime, itemEmergeTime) / 1000.0 > 7) {
            // itemEmergeTime 을 tempTime 으로 바꿔줌으로써.. 계속해서 5차이나면 map 을 새로 프린트 하도록..
            itemEmergeTime = tempTime;

            // food 의 setFoodInfo() 함수를 사용해서 food 의 정보 초기화..
            // random 으로 임의의 수 만들고 함수로 넘겨주기..
            food.setFoodInfo(posXDis(randomPosX), posYDis(randomPosY), idDis(randomId));
            mapInstance.changeMapItemState(mapInstance.mapLevel, food.foodPosX, food.foodPosY, food.prevPosX, food.prevPosY, food.foodId);


            // 아이템 정보 변경 해 준 다음 맵 새로 그리기..
            mapInstance.setMap();


            // 시간이 지나면 맵에서 없애기 위해 이전 위치를 저장하기..
            food.prevPosX = food.foodPosX;
            food.prevPosY = food.foodPosY;

        }


        if (mapInstance.mapLevel == 2 && difftime(tempTime, gateEmergeTime) / 1000.0 > 10) {
            // gateTime 을 tempTime 으로 바꿔줌으로써.. 계속해서 10 차이 나면 map 을 새로 프린트 하도록..
            gateEmergeTime = tempTime;

            if (!gateMapStarted) {
                // 맨 처음 gateMap 으로 진입했을 땐 변경사항 없도록 분기 나눠줌..
                gateMapStarted = true;
            }
            else {

                gate1.isEdgeGate = false;
            }

            if ((gate2.gatePosX == 0 || gate2.gatePosX == 45) || (gate2.gatePosY == 0 || gate2.gatePosY == 22)) {
                gate2.isEdgeGate = true;
            }
            else {
                gate2.isEdgeGate = false;
            }

            // 뱀이 게이트를 통과하고 있지 않을 때, 게이트의 위치를 바꿔주는 로직을 실행할 것임..
            if (!gate1.isSnakeMoved) {
                uniform_int_distribution<int> vecIndexDis(0, gate1.gateEmergeVec.size() - 1);
                int tempGate1Index = vecIndexDis(randomVecIndex);
                int tempGate2Index = vecIndexDis(randomVecIndex);

                gate1.gatePosX = gate1.gateEmergeVec[tempGate1Index].gateEmergeEnablePosX;
                gate1.gatePosY = gate1.gateEmergeVec[tempGate1Index].gateEmergeEnablePosY;
                gate2.gatePosX = gate2.gateEmergeVec[tempGate2Index].gateEmergeEnablePosX;
                gate2.gatePosY = gate2.gateEmergeVec[tempGate2Index].gateEmergeEnablePosY;

                GateEmergeEnablePos tempGate1{ gate1.gatePosX, gate1.gatePosY };
                GateEmergeEnablePos tempGate2{ gate2.gatePosX, gate2.gatePosY };

                mapInstance.changeMapGateState(mapInstance.mapLevel, tempGate1, tempGate2, gate1.prevGatePosX, gate1.prevGatePosY, gate2.prevGatePosX, gate2.prevGatePosY);

                gate1.setGateEmergeVec(mapInstance.map, mapInstance.mapLevel);
                gate2.setGateEmergeVec(mapInstance.map, mapInstance.mapLevel);


                // 게이트 정보 변경 해 준 다음 맵 새로 그리기..
                mapInstance.setMap();


                // 시간이 지나면 맵에서 없애기 위해 이전 위치를 저장하기..
                gate1.prevGatePosX = gate1.gatePosX;
                gate1.prevGatePosY = gate1.gatePosY;
                gate2.prevGatePosX = gate2.gatePosX;
                gate2.prevGatePosY = gate2.gatePosY;
            }
        }



        if (food.foodId == 3 && (snake.head.x == food.foodPosX && snake.head.y == food.foodPosY)) {
            // 뱀의 머리랑 싱싱한사과가 부딪히면(== 뱀의 머리랑 싱싱한사과의 위치가 같으면)
            // snake 의 snakeTouchApple() 함수 실행하기..
            snake.snakeTouchFreshApple();

            Info.setCurrentLength(snake.snakeSize);
            Info.setMaxLen(snake.snakeSize);
            Info.IncreaseGetGrowthItem();
        }

        if (food.foodId == 4 && (snake.head.x == food.foodPosX && snake.head.y == food.foodPosY)) {
            // 뱀의 머리랑 썩은사과가 부딪히면(== 뱀의 머리랑 썩은사과의 위치가 같으면)
            // 뱀의 사이즈가 줄어듦.
            snake.snakeTouchPoisonApple();

            Info.setCurrentLength(snake.snakeSize);
            Info.IncreaseGetPoisonItem();
        }

        if (snake.snakeSize < 3) {
            snake.isDie = true;
            gotoxy(MAPWIDTH * 2 + 8, 6);
            cout << "Game Over!";

            // 뱀의 크기가 3보다 작아지면 게임 끝~~!
            break;
        }

        // 뱀의 머리랑 뱀의 몸통이 부딪히면 게임 종료!
        // snake.moveX==0, snake.moveY==0 이 동시에 만족될 때는 게임 시작 직후 방향키를 누르지 않은채로 멈춰있는 상태..
        // 이때는 머리랑 몸이랑 닿아도 죽으면 안되니까 따로 조건문 걸어준 것..
        if (snake.moveX == 0 && snake.moveY == 0) {}
        else {
            for (int i = 0; i < snake.snakeSize - 1; i++) {
                if ((snake.head.x == snake.body[i].x) && (snake.head.y == snake.body[i].y)) {
                    snake.isDie = true;
                    gotoxy(MAPWIDTH * 2 + 8, 6);
                    cout << "game Over!";
                    return 0;
                }
            }
        }

        // 만약 뱀이 gate1 으로 들어갈 때..
        // 만약 현재 뱀의 이동 방향이 LEFT 면 snake 의 moveX=-1, moveY=0 임..
        // 이를 gate 위치에 빼주면, 게이트 위치의 바로 오른쪽 위치를 가리킴..
        if ((snake.head.y + snake.moveY == gate1.gatePosY) && (snake.head.x + snake.moveX == gate1.gatePosX)) {
            snake.snakeTouchGate(&gate1, &gate2, snake.moveX, snake.moveY, &snake);
            Info.setGateUse(snake.gateCount - 1);
            Info.IncreaseGateCount();

        }
        else if ((snake.head.y + snake.moveY == gate2.gatePosY) && (snake.head.x + snake.moveX == gate2.gatePosX)) {
            snake.snakeTouchGate(&gate2, &gate1, snake.moveX, snake.moveY, &snake);
            Info.setGateUse(snake.gateCount - 1);
            Info.IncreaseGateCount();

        }


        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            snake.setPos(Snake::UP);
        }
        else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            snake.setPos(Snake::DOWN);
        }
        else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            snake.setPos(Snake::LEFT);
        }
        else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            snake.setPos(Snake::RIGHT);
        }

        snake.move(snake.moveX, snake.moveY);


        // 벽 닿으면 죽는거. 편의상 일단 주석처리
        //if (mapInstance.map[mapInstance.mapLevel][snake.head.y][snake.head.x]==1) {
        //    gotoxy(MAPWIDTH * 2 + 8, 6);
        //    std::cout << "Game Over" << std::endl;
        //    break; // 게임 종료
        //}
        if (snake.isDie) {
            gotoxy(MAPWIDTH * 2 + 8, 6);
            std::cout << "Game Over!" << std::endl;
            break; // 게임 종료
        }
        Info.ShowInfo();

        Info.Mission();
        if (Info.IsGoal()) {
            gotoxy(MAPWIDTH * 2 + 8, 6);
            std::cout << "Mission Complete!" << "\n";
            Info.ReSetInfo();
            snake.ReSetSnake();
            mapInstance.setMap();
        }
        Sleep(100);
    }


    return 0;
}