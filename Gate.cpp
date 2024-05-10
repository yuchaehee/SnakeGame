#include "Gate.h"
#include <vector>



Gate::Gate(int x, int y)
    : gatePosX(x), gatePosY(y), gateEmergeVec{}
{
    prevGatePosX = gatePosX;
    prevGatePosY = gatePosY;
}

void Gate::setGateEmergeVec(int map[][23][46], int mapLevel) {
    // 새로운 맵을 반영해야 하늠로 기존의 게이트 생성 가능 위치 인스턴스는 다 버리기..
    gateEmergeVec.clear();

    for (int i = 0; i < 23; i++) {
        for (int j = 0; j < 46; j++) {
            if (map[mapLevel][i][j] == 1) {
                gateEmergeVec.emplace_back(j, i);
            }
        }
    }
}