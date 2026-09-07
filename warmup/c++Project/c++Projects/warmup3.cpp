#include <iostream>
#include <conio.h>

using namespace std;

const int gListSize{ 10 };

// 3차원 좌표 변수
struct Vec3
{
	int x;
	int y;
	int z;

	Vec3();
	Vec3(int, int, int);

	friend ostream& operator<<(ostream&, const Vec3&);
};

struct Element
{
	Vec3 data{};
	bool isEmpty{ true };

	Element& operator=(const Vec3&);
};

// 3차원 좌표 정적 리스트
struct Vec3List
{
	Element vec3List[gListSize]{};
	int top{};
	int bottom{};

	Vec3List() = default;
	Vec3List(initializer_list<Vec3>);

	void printAll() const;
	void inputTop(const Vec3&);
	void deleteTop();
	void inputBottom(const Vec3&);
	void deleteBottom();
};

int main()
{
	Vec3List vec3List{ {}, {1, 2, 3} };

	vec3List.printAll();

	return 0;
}

// Vec3 맴버함수 정의
Vec3::Vec3()
	: x(), y(), z()
{
}

Vec3::Vec3(int _x, int _y, int _z)
	: x(_x), y(_y), z(_z)
{
}

ostream& operator<<(ostream& os, const Vec3& rhs)
{
	os << "(" << rhs.x << ", " << rhs.y << ", " << rhs.z << ")";	// (x, y, z)
	return os;
}

// Element 맴버함수 정의
Element& Element::operator=(const Vec3& other)
{
	data.x = other.x;
	data.y = other.y;
	data.z = other.z;

	return *this;
}

// Vec3List 맴버함수 정의
Vec3List::Vec3List(initializer_list<Vec3> initList)
{
	int count{};
	for (const auto& item : initList)
	{
		if (count >= gListSize) break;
		vec3List[count].isEmpty = false;
		vec3List[count++] = item;
		top = count;
	}
}

void Vec3List::printAll() const
{
	cout << "index\tdata" << endl;
	for (int i = 1; i < gListSize + 1; ++i)
	{
		cout << gListSize - i << '\t';
		if (!vec3List[gListSize - i].isEmpty) cout << vec3List[gListSize - i].data;
		cout << endl;
	}
	cout << top << '\t' << bottom << endl;
}

void Vec3List::inputTop(const Vec3& vec3)
{

}

void Vec3List::deleteTop()
{
}

void Vec3List::inputBottom(const Vec3&)
{
}

void Vec3List::deleteBottom()
{
}

