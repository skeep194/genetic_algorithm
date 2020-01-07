#include <array>
#include <vector>
struct status
{
    double atk, dfk, avg; //공, 방, 민첩
    status() : atk(1000),dfk(1000),avg(1000) {}
    status(int _atk, int _dfk, int _avg) : atk(_atk), dfk(_dfk), avg(_avg) {}
};

struct pos2
{
    int y, x;
};

class chromosome
{
private:
    status preference, stat; //각 스탯 선호도, 실제 스탯 수치
    int HP;
    pos2 pos, move; //pos : 현재 위치 move : 이동 목적지, 갈 수 있는 최단 경로를 검색해 움직임, 중간에 막힐 시 다른 행동 설정
    
    std::array<double,3> tendency; // 1 - 100 의 수치, 전투 선호도, HP 비율에 따른 대기 혹은 이동 선호도, 턴당 행동 바꾸는 비율
    bool isFight; //'전투' 상태 여부
public:
    chromosome();
    chromosome operator*(const chromosome& c); //두 유전자 교배
    unsigned long fitness();//마지막에 결과값 계산 : 스탯 랭킹 딜량 등을 고려
    void mutate();

    void turn(); //턴당 행동 결정
    void sight(); //현재 위치에서 시야에 있는 오브젝트 확인
};

class gameboard
{
private:
    std::vector<std::vector<char>> board;
public:
    gameboard();
    gameboard(int y, int x)
    {
        board.assign(y,std::vector<char>(x,'.'));
    }
};

chromosome::chromosome()
{

}

/*
각 턴당 할 수 있는 내용
1. 대기 - HP 회복됨
2. 이동
3. 공격 (유효 사거리 안일 시)

2. 이동 시 가능한 행동 (비전투)
    2 - 1 : 이길 수 있을 법한 상대를 향해 이동 - 스탯, HP 등 상황 종합적으로 고려
    2 - 2 : 질 법한 상대를 피해 이동
    2 - 3 : 파밍 스팟으로 이동
2. 이동 시 가능한 행동 (전투)
    2 - 1 : 상대방 쪽으로 이동
    2 - 2 : 상대방의 반대 방향으로 이동

3. 공격받을 시 가능한 행동
    3 - 1 : 맞서 싸움
    3 - 2 : 도망감
    3 - 3 : 방어

각 캐릭터는 공격하거나 공격받을 시 n턴간 '전투' 상태가 됨
이 상태에서는 파밍 불가
서로 n턴간 공격하지 않으면 '전투' 상태가 해제됨
*/
