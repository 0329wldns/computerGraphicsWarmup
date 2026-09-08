#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>

using namespace std;

// 점 (x, y, z) 데이터를 저장하는 구조체
struct Point {
    int x;
    int y;
    int z;
    bool occupied; // 데이터 존재 여부
};

// 정렬 출력용 임시 구조체
struct SortedPoint {
    int x;
    int y;
    int z;
    double dist;
};

// 전역 변수로 관리하는 10개의 점 데이터 및 출력 상태
Point pointList[10];
bool isSortedMode = false; // f 명령어 토글 플래그

// 초기화 함수
void initList() {
    for (int i = 0; i < 10; ++i) {
        pointList[i].x = 0;
        pointList[i].y = 0;
        pointList[i].z = 0;
        pointList[i].occupied = false;
    }
}

// 원점으로부터의 거리 계산
double getOriginDistance(const Point& p) {
    return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

// 두 점 사이의 거리 계산
double getPointDistance(const Point& p1, const Point& p2) {
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    int dz = p1.z - p2.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

// 저장된 점의 개수 세기 (명령어 a 지원)
int getPointCount() {
    int count = 0;
    for (int i = 0; i < 10; ++i) {
        if (pointList[i].occupied) count++;
    }
    return count;
}

// 리스트 상태 출력 함수
void printList() {
    cout << "\n===============================\n";

    // f 명령어가 활성화된 경우 (원점 거리 기준 오름차순 출력)
    if (isSortedMode) {
        SortedPoint sortedArray[10];
        int count = 0;

        // 저장된 점 추출 및 원점 거리 계산
        for (int i = 0; i < 10; ++i) {
            if (pointList[i].occupied) {
                sortedArray[count].x = pointList[i].x;
                sortedArray[count].y = pointList[i].y;
                sortedArray[count].z = pointList[i].z;
                sortedArray[count].dist = getOriginDistance(pointList[i]);
                count++;
            }
        }

        // 버블 정렬 (거리 오름차순)
        for (int i = 0; i < count - 1; ++i) {
            for (int j = 0; j < count - 1 - i; ++j) {
                if (sortedArray[j].dist > sortedArray[j + 1].dist) {
                    SortedPoint temp = sortedArray[j];
                    sortedArray[j] = sortedArray[j + 1];
                    sortedArray[j + 1] = temp;
                }
            }
        }

        // 인덱스 9번부터 0번까지 출력
        for (int i = 9; i >= 0; --i) {
            cout << i;
            if (i < count) {
                cout << " | " << sortedArray[i].x << sortedArray[i].y << sortedArray[i].z
                    << "  (원점 거리: " << fixed << setprecision(2) << sortedArray[i].dist << ")";
            }
            cout << "\n";
        }
    }
    // 일반 출력 모드
    else {
        for (int i = 9; i >= 0; --i) {
            cout << i;
            if (pointList[i].occupied) {
                cout << " | " << pointList[i].x << pointList[i].y << pointList[i].z;
            }
            cout << "\n";
        }
    }
    cout << "===============================\n";
}

// 리스트의 맨 위에 데이터 추가 (+)
void insertTop(int x, int y, int z) {
    if (getPointCount() >= 10) {
        cout << "[경고] 리스트가 가득 찼습니다.\n";
        return;
    }

    int maxIndex = -1;
    for (int i = 9; i >= 0; --i) {
        if (pointList[i].occupied) {
            maxIndex = i;
            break;
        }
    }

    if (maxIndex == -1) {
        pointList[0] = { x, y, z, true };
    }
    else if (maxIndex < 9) {
        pointList[maxIndex + 1] = { x, y, z, true };
    }
    else {
        // 9번 칸까지 찼는데 빈 칸이 있는 경우 0번부터 빈 곳 탐색
        for (int i = 0; i < 9; ++i) {
            if (!pointList[i].occupied) {
                pointList[i] = { x, y, z, true };
                break;
            }
        }
    }
}

// 리스트의 맨 위에서 데이터 삭제 (-)
void deleteTop() {
    for (int i = 9; i >= 0; --i) {
        if (pointList[i].occupied) {
            pointList[i].occupied = false;
            return;
        }
    }
    cout << "[알림] 삭제할 데이터가 없습니다.\n";
}

// 리스트의 맨 아래에 데이터 입력 (e)
void insertBottom(int x, int y, int z) {
    // 한 칸씩 위로 밀기
    for (int i = 9; i > 0; --i) {
        pointList[i] = pointList[i - 1];
    }
    pointList[0] = { x, y, z, true };
}

// 리스트의 맨 아래 데이터 삭제 (d)
void deleteBottom() {
    if (pointList[0].occupied) {
        pointList[0].occupied = false;
    }
    else {
        for (int i = 0; i < 10; ++i) {
            if (pointList[i].occupied) {
                pointList[i].occupied = false;
                break;
            }
        }
    }
}

// 위치를 한 칸씩 아래로 이동 (b)
void shiftDown() {
    Point temp = pointList[0];
    for (int i = 0; i < 9; ++i) {
        pointList[i] = pointList[i + 1];
    }
    pointList[9] = temp;
}

// 두 점 간의 거리 조합 계산 및 최장/최단 출력 (g)
void calculatePairDistances() {
    int total = getPointCount();
    if (total < 2) {
        cout << "[알림] 계산을 위해 최소 2개 이상의 점이 필요합니다.\n";
        return;
    }

    int activeIndices[10];
    int count = 0;
    for (int i = 0; i < 10; ++i) {
        if (pointList[i].occupied) {
            activeIndices[count++] = i;
        }
    }

    int minIdx1 = -1, minIdx2 = -1;
    int maxIdx1 = -1, maxIdx2 = -1;
    double minDist = 1e9;
    double maxDist = -1.0;

    for (int i = 0; i < count; ++i) {
        for (int j = i + 1; j < count; ++j) {
            int p1 = activeIndices[i];
            int p2 = activeIndices[j];
            double d = getPointDistance(pointList[p1], pointList[p2]);

            if (d < minDist) {
                minDist = d;
                minIdx1 = p1;
                minIdx2 = p2;
            }
            if (d > maxDist) {
                maxDist = d;
                maxIdx1 = p1;
                maxIdx2 = p2;
            }
        }
    }

    cout << "\n[가장 가까운 두 점]\n";
    cout << "(" << pointList[minIdx1].x << ", " << pointList[minIdx1].y << ", " << pointList[minIdx1].z << ") 와 "
        << "(" << pointList[minIdx2].x << ", " << pointList[minIdx2].y << ", " << pointList[minIdx2].z << ")\n";
    cout << "거리: " << fixed << setprecision(2) << minDist << "\n\n";

    cout << "[가장 먼 두 점]\n";
    cout << "(" << pointList[maxIdx1].x << ", " << pointList[maxIdx1].y << ", " << pointList[maxIdx1].z << ") 와 "
        << "(" << pointList[maxIdx2].x << ", " << pointList[maxIdx2].y << ", " << pointList[maxIdx2].z << ")\n";
    cout << "거리: " << fixed << setprecision(2) << maxDist << "\n";
}

int main() {
    initList();
    string cmdInput;

    while (true) {
        printList();
        cout << "(명령어 입력): ";
        if (!(cin >> cmdInput)) break;

        char cmd = cmdInput[0];

        if (cmd == 'q') {
            cout << "프로그램을 종료합니다.\n";
            break;
        }

        if (cmd == '+') {
            int x = 0, y = 0, z = 0;
            if (cmdInput.length() >= 4) {
                x = cmdInput[1] - '0';
                y = cmdInput[2] - '0';
                z = cmdInput[3] - '0';
            }
            else {
                cin >> x >> y >> z;
            }
            insertTop(x, y, z);
        }
        else if (cmd == 'e') {
            int x = 0, y = 0, z = 0;
            if (cmdInput.length() >= 4) {
                x = cmdInput[1] - '0';
                y = cmdInput[2] - '0';
                z = cmdInput[3] - '0';
            }
            else {
                cin >> x >> y >> z;
            }
            insertBottom(x, y, z);
        }
        else if (cmd == '-') {
            deleteTop();
        }
        else if (cmd == 'd') {
            deleteBottom();
        }
        else if (cmd == 'a') {
            cout << "\n[저장된 점의 개수]: " << getPointCount() << "개\n";
        }
        else if (cmd == 'b') {
            shiftDown();
        }
        else if (cmd == 'c') {
            initList();
            cout << "\n[리스트를 초기화했습니다.]\n";
        }
        else if (cmd == 'f') {
            isSortedMode = !isSortedMode; // 토글 동작
        }
        else if (cmd == 'g') {
            calculatePairDistances();
        }
        else {
            cout << "[오류] 올바르지 않은 명령어입니다.\n";
        }
    }

    return 0;
}