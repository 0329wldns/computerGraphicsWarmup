#include <iostream>
#include <conio.h>
#include <cmath>
#include <iomanip>
#include <limits>
#include <windows.h> // system("cls") 사용을 위함

#undef max

using namespace std;

bool gToggleA{ false };
bool gToggleF{ false };
bool gToggleG{ false };

// 1. 단일 점을 표현하는 구조체
struct Point 
{
	int x, y, z;
	bool isEmpty;

	// 원점과의 거리를 반환하는 유틸리티 함수
	double distFromOrigin() const 
	{
		return sqrt(x * x + y * y + z * z);
	}
};

// 2. 점들의 리스트와 연산을 관리하는 관리용 구조체
struct PointList 
{
	Point list[10];
	int count;
	int top; // 논리적 '맨 위' 위치 추적용

	// 생성자: 초기화
	PointList() 
	{
		clearList();
	}

	// [c 명령어] 리스트 비우기 및 초기화
	void clearList() 
	{
		for (int i = 0; i < 10; i++) list[i].isEmpty = true;
		count = 0;
		top = -1;
	}

	// [+ 명령어] 리스트의 맨 위에 입력
	void addTop(int x, int y, int z) 
	{
		if (count < 10) 
		{
			int target{ (top + 1) % 10 };
			while (!list[target].isEmpty) target = (target + 1) % 10;
			list[target] = { x, y, z, false };
			top = target;
			count++;
		}
	}

	// [- 명령어] 리스트의 맨 위에서 삭제
	void deleteTop() 
	{
		if (count > 0) 
		{
			for (int step = 0; step < 10; step++) 
			{
				int idx = (top - step + 10) % 10;
				if (!list[idx].isEmpty) 
				{
					list[idx].isEmpty = true;
					count--;
					top = (idx - 1 + 10) % 10;
					break;
				}
			}
		}
	}

	// [e 명령어] 리스트의 맨 아래에 입력 (데이터가 위로 밀림)
	void addBottom(int x, int y, int z) 
	{
		if (!list[9].isEmpty) count--; // 맨 위(9번)가 차있으면 밀려나서 삭제됨
		
		// 모든 데이터를 한 칸씩 위로 이동
		for (int i = 8; i >= 0; i--) list[i + 1] = list[i];
		list[0] = { x, y, z, false };
		count++;
		top = (top + 1) % 10;
	}

	// [d 명령어] 리스트의 맨 아래에서 삭제
	void deleteBottom() 
	{
		if (count > 0) 
		{
			for (int i = 0; i < 10; i++) 
			{
				if (!list[i].isEmpty) 
				{
					list[i].isEmpty = true;
					count--;
					break;
				}
			}
		}
	}

	// [b 명령어] 리스트 위치 한 칸씩 내림
	void shiftDown() 
	{
		if (count > 0) 
		{
			Point temp = list[0];
			for (int i = 0; i < 9; i++) 
			{
				list[i] = list[i + 1];
			}
			list[9] = temp;
			top = (top - 1 + 10) % 10;
		}
	}

	// [a 명령어] 저장된 개수 반환
	int getCount() const 
	{
		return count;
	}

	// [f 모드용 / 기본 모드용] 리스트 출력
	void printList() 
	{
		if (gToggleF) {
			Point temp[10];
			int t_count = 0;
			for (int i = 0; i < 10; i++) 
			{
				if (!list[i].isEmpty) temp[t_count++] = list[i];
			}
			// 버블 정렬 (오름차순)
			for (int i = 0; i < t_count - 1; i++) 
			{
				for (int j = i + 1; j < t_count; j++) 
				{
					if (temp[i].distFromOrigin() > temp[j].distFromOrigin()) 
					{
						Point swap = temp[i];
						temp[i] = temp[j];
						temp[j] = swap;
					}
				}
			}

			for (int i = 9; i >= 0; i--) 
			{
				cout << i << " | ";
				if (i < t_count) 
				{
					cout << temp[i].x << " " << temp[i].y << " " << temp[i].z
						<< " (dist: " << fixed << setprecision(2) << temp[i].distFromOrigin() << ")";
				}
				cout << "\n";
			}
		}
		else {
			for (int i = 9; i >= 0; i--) 
			{
				cout << i << " | ";
				if (!list[i].isEmpty) cout << list[i].x << " " << list[i].y << " " << list[i].z;
				cout << "\n";
			}
		}
	}

	// [g 명령어용] 점들 간의 거리 계산 및 출력
	void printDistancePairs() 
	{
		if (count < 2) 
		{
			cout << "[알림] 거리를 계산하려면 2개 이상의 점이 필요합니다.\n";
			return;
		}
		
		double max_d = numeric_limits<double>::lowest(), min_d = numeric_limits<double>::max();
		Point p_max1, p_max2, p_min1, p_min2;

		for (int i = 0; i < 10; i++) 
		{
			if (list[i].isEmpty) continue;
			for (int j = i + 1; j < 10; j++) 
			{
				if (list[j].isEmpty) continue;
				double d = sqrt(pow(list[i].x - list[j].x, 2) +
					pow(list[i].y - list[j].y, 2) +
					pow(list[i].z - list[j].z, 2));
				if (d > max_d) { max_d = d; p_max1 = list[i]; p_max2 = list[j]; }
				if (d < min_d) { min_d = d; p_min1 = list[i]; p_min2 = list[j]; }
			}
		}
		cout << "[거리 계산 결과]\n";
		cout << "가장 먼 두 점: (" << p_max1.x << "," << p_max1.y << "," << p_max1.z << ") & ("
			<< p_max2.x << "," << p_max2.y << "," << p_max2.z << ") -> 거리: " << fixed << setprecision(2) << max_d << "\n";
		cout << "가장 가까운 두 점: (" << p_min1.x << "," << p_min1.y << "," << p_min1.z << ") & ("
			<< p_min2.x << "," << p_min2.y << "," << p_min2.z << ") -> 거리: " << fixed << setprecision(2) << min_d << "\n";
	}
};

int main() 
{
	PointList pointList; // 점 리스트 관리 구조체 인스턴스 생성

	while (true) {
		system("cls");

		// 구조체의 출력 멤버함수 호출
		pointList.printList();

		cout << "========================================\n";

		if (gToggleA) 
		{
			cout << "[알림] 현재 리스트에 저장된 점의 개수: " << pointList.getCount() << "개\n";
			gToggleA = false;
		}

		if (gToggleG) pointList.printDistancePairs();

		cout << "\n명령어 입력 (+, -, e, d, a, b, c, f, g, q)";
		char cmd = _getch();

		switch (cmd)
		{
		case '+':
		{
			cout << "\n(x y z 값을 띄어쓰기로 구분하여 입력): ";
			int x, y, z;
			if (!(cin >> x >> y >> z))
			{
				cin.clear();
				while (cin.get() != '\n');
				continue;
			}
			pointList.addTop(x, y, z);
			break;
		}
		case 'e':
		{
			cout << "\n(x y z 값을 띄어쓰기로 구분하여 입력): ";
			int x, y, z;
			if (!(cin >> x >> y >> z))
			{
				cin.clear();
				while (cin.get() != '\n');
				continue;
			}
			pointList.addBottom(x, y, z);
			break;
		}
		case '-':
			pointList.deleteTop();
			break;
		case 'd':
			pointList.deleteBottom();
			break;
		case 'a':
			gToggleA = true;
			break;
		case 'b':
			pointList.shiftDown();
			break;
		case 'c':
			pointList.clearList();
			break;
		case 'f':
			gToggleF = !gToggleF;
			break;
		case 'g':
			gToggleG = !gToggleG;
			break;
		case 'q':
			return 0;
			break;
		default:
			break;
		}
	}
	return 0;
}