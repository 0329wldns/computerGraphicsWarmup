#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// 3차원 정점(Vertex) 구조체
struct Vector3
{
    float x{};
    float y{};
    float z{};

    // 좌표값 중복 체크를 위한 비교 연산자 오버로딩
    bool isEqual(Vector3 other) const
    {
        return (x == other.x && y == other.y && z == other.z);
    }
};

// 2차원 텍스처(Texture) 구조체
struct Vector2
{
    float u{};
    float v{};
};

// 정점 및 텍스처 인덱스 묶음 구조체
struct VertexIndex
{
    int vIdx{};
    int vtIdx{};
};

// 면(Face) 구조체
struct Face
{
    vector<VertexIndex> indices{};
    bool isValid{ true };
    string errorMessage{};
};

// OBJ 파일 파싱 및 데이터 관리를 담당하는 관리형 구조체 (클래스 배제)
struct ObjParser
{
    vector<Vector3> vertices{};
    vector<Vector2> texCoords{};
    vector<Face> faces{};

    // 파일 읽기 및 파싱
    void loadFromFile(string fileName)
    {
        ifstream file(fileName);
        if (!file.is_open())
        {
            cout << "[에러] " << fileName << " 파일을 열 수 없습니다.\n";
            return;
        }

        string line{};
        int lineNum{};

        while (getline(file, line))
        {
            ++lineNum;
            if (line.empty()) continue; // 한 줄 코드

            processLine(line, lineNum);
        }

        file.close();
        cout << "[성공] 파일 읽기 및 파싱 완료.\n";
    }

    // 한 줄 파싱 로직 (문자열 처리 및 예외 상황 4번: 허용되지 않는 문자 체크)
    void processLine(string line, int lineNum)
    {
        stringstream ss(line);
        string type{};
        ss >> type;

        if (type == "#") return; // 주석 무시

        if (type == "v")
        {
            Vector3 vec{};
            if (!(ss >> vec.x >> vec.y >> vec.z))
            {
                cout << "[경고] " << lineNum << "번 줄 'v' 데이터에 허용되지 않는 문자가 포함되어 있습니다.\n";
                return;
            }
            vertices.push_back(vec);
        }
        else if (type == "vt")
        {
            Vector2 tex{};
            if (!(ss >> tex.u >> tex.v))
            {
                cout << "[경고] " << lineNum << "번 줄 'vt' 데이터에 허용되지 않는 문자가 포함되어 있습니다.\n";
                return;
            }
            texCoords.push_back(tex);
        }
        else if (type == "f")
        {
            Face face{};
            string token{};

            while (ss >> token)
            {
                stringstream tokenSs(token);
                string vStr{}, vtStr{};
                VertexIndex vIndex{};

                // '/' 문자를 기준으로 분리
                getline(tokenSs, vStr, '/');
                getline(tokenSs, vtStr, '/');

                // 숫자 변환 중 예외 발생 시 (허용되지 않는 문자) isValid를 false로 처리
                try
                {
                    vIndex.vIdx = stoi(vStr);
                    if (!vtStr.empty()) vIndex.vtIdx = stoi(vtStr);
                }
                catch (...)
                {
                    face.isValid = false;
                    face.errorMessage = "허용되지 않는 문자 포함";
                }

                face.indices.push_back(vIndex);
            }

            validateFace(face);
            faces.push_back(face);
        }
    }

    // 삼각형 데이터 무결성 검증 (예외 상황 1, 2, 3, 5번 체크)
    void validateFace(Face& face)
    {
        if (!face.isValid) return; // 이미 에러가 났다면 스킵

        // 1) 삼각형 구성 여부 체크 (꼭짓점이 3개가 아닌 경우)
        if (face.indices.size() != 3)
        {
            face.isValid = false;
            face.errorMessage = "에러메시지: 삼각형이 이루어지지 않습니다 (꼭짓점 개수 오류)";
            return;
        }

        // 3) 인덱스 범위 체크 (0 이하이거나 저장된 정점 수를 초과하는 경우)
        for (int i{}; i < 3; ++i)
        {
            int vIdx{ face.indices[i].vIdx };
            int vtIdx{ face.indices[i].vtIdx };

            int vSize{ static_cast<int>(vertices.size()) };
            int vtSize{ static_cast<int>(texCoords.size()) };

            if (vIdx < 1 || vIdx > vSize || (vtIdx != 0 && (vtIdx < 1 || vtIdx > vtSize)))
            {
                face.isValid = false;
                face.errorMessage = "에러메시지: 인덱스 범위 외의 값 참조";
                return;
            }
        }

        // 2) 정점의 좌표값 중복 체크 (세 개의 좌표값이 모두 달라야 함)
        Vector3 p1{ vertices[face.indices[0].vIdx - 1] };
        Vector3 p2{ vertices[face.indices[1].vIdx - 1] };
        Vector3 p3{ vertices[face.indices[2].vIdx - 1] };

        if (p1.isEqual(p2) || p2.isEqual(p3) || p3.isEqual(p1))
        {
            face.isValid = false;
            face.errorMessage = "에러메시지: 삼각형 정점의 값이 중복됨";
            return;
        }
    }

    // 파일로 저장 및 양식 출력
    void saveToFile(string fileName)
    {
        ofstream outFile(fileName);
        if (!outFile.is_open())
        {
            cout << "[에러] " << fileName << " 파일을 생성할 수 없습니다.\n";
            return;
        }

        outFile << fixed << setprecision(1);

        int faceCount{ static_cast<int>(faces.size()) };
        for (int i{}; i < faceCount; ++i)
        {
            Face f{ faces[i] };
            outFile << "Face " << (i + 1) << " (";

            // 유효하지 않은 면인 경우 에러 메시지 출력 후 다음으로 넘어감
            if (!f.isValid)
            {
                outFile << ") : " << f.errorMessage << "\n\n";
                continue;
            }

            // 올바른 삼각형인 경우 요구사항 양식대로 출력
            outFile << f.indices[0].vIdx << ", " << f.indices[1].vIdx << ", " << f.indices[2].vIdx << "): \n";

            // Vertex 출력
            outFile << "vertex ";
            for (int j{}; j < 3; ++j)
            {
                Vector3 v{ vertices[f.indices[j].vIdx - 1] }; // OBJ 인덱스는 1부터 시작하므로 -1
                outFile << "(" << v.x << ", " << v.y << ", " << v.z << ") ";
            }
            outFile << "\n";

            // Texture 출력 (존재할 경우)
            if (f.indices[0].vtIdx != 0)
            {
                outFile << "texture ";
                for (int j{}; j < 3; ++j)
                {
                    Vector2 vt{ texCoords[f.indices[j].vtIdx - 1] };
                    outFile << "(" << vt.u << ", " << vt.v << ") ";
                }
                outFile << "\n";
            }

            outFile << "No duplicate vertex value\n\n";
        }

        outFile.close();
        cout << "[성공] 파싱 결과를 " << fileName << "에 저장했습니다.\n";
    }
};

// --- 함수의 원형(Prototype) 선언부 ---
void runParser();

// --- Main 함수 ---
int main()
{
    runParser();
    return 0;
}

// --- 함수의 정의부 (Definition) ---
void runParser()
{
    ObjParser parser{};
    string inputFileName{};
    string outputFileName{};

    cout << "========================================\n";
    cout << "     3D OBJ 파싱 및 검증 프로그램\n";
    cout << "========================================\n";

    cout << "읽어올 파일의 이름을 입력하세요 (예: data1.txt): ";
    cin >> inputFileName;

    parser.loadFromFile(inputFileName);

    // 파싱된 데이터가 있을 경우에만 저장 수행
    if (parser.faces.size() > 0)
    {
        cout << "결과를 저장할 파일의 이름을 입력하세요 (예: output.txt): ";
        cin >> outputFileName;

        parser.saveToFile(outputFileName);
    }
    else cout << "[알림] 저장할 Face 데이터가 없습니다.\n";
}