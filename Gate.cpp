#include "Gate.h"
#include <vector>



Gate::Gate(int x, int y)
    : gatePosX(x), gatePosY(y), gateEmergeVec{}
{
    prevGatePosX = gatePosX;
    prevGatePosY = gatePosY;
}

void Gate::setGateEmergeVec(int map[][23][46], int mapLevel) {
    // ���ο� ���� �ݿ��ؾ� �ϴƷ� ������ ����Ʈ ���� ���� ��ġ �ν��Ͻ��� �� ������..
    gateEmergeVec.clear();

    for (int i = 0; i < 23; i++) {
        for (int j = 0; j < 46; j++) {
            if (map[mapLevel][i][j] == 1) {
                gateEmergeVec.emplace_back(j, i);
            }
        }
    }
}