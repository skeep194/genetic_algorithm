#ifndef GENETIC_FIGHTER_H
#define GENETIC_FIGHTER_H
/*
각 유전자가 가질 수 있는 정보 객체
*/
#include <array>
#include <random>
#include <queue>
#include <map>
#include <algorithm>
#include <set>

class chromosome
{
private:
    int uniq_num; //유전체 식별변호

    glib::status preference, stat; //각 스탯 선호도, 실제 스탯 수치
    int HP;
    glib::pos2 pos; //pos : 현재 위치 
    std::pair<int, glib::pos2> movetarget;//movetarget first : 개체 식별 번호, second : 이동 목적지 - 갈 수 있는 최단 경로를 검색해 움직임, 중간에 막힐 시 다른 행동 설정
    std::list<glib::pos2> route; //movetarget가 가리키는 곳을 가는 경로

    std::array<int,3> tendency; // 1 - 100 의 수치, 전투 선호도, HP 비율에 따른 대기 혹은 이동 선호도, 턴당 행동 바꾸는 비율
    std::map<int,int> isFight; //싸우는 적 리스트, first : 식별번호 second : 지속시간
    int prev; //저번 턴에 한 행동 1 : 전투 - 상대를 향한 이동, 2 : 전투 - 도망, 3 : 일반 - 상대를 향한 이동, 4 : 일반 - 파밍 스팟을 향한 이동
    unsigned int target(gameboard& gb,std::set<glib::pos2> evade = std::set<glib::pos2>()); //이동할 목표 좌표를 찍어줌, evade : 피할 목표 배열 return : 거리

    std::random_device rd;
    std::mt19937 engine;
    //유전체의 랜덤 엔진 초기화

public:
    int get_num(){return uniq_num;} //식별번호 리턴

    chromosome(int _uniq_num);
    chromosome operator*(const chromosome& c); //두 유전자 교배
    unsigned long fitness();//마지막에 결과값 계산 : 스탯 랭킹 딜량 등을 고려
    void mutate();

    void turn_act(); //턴당 행동 결정
    void turn(const gameboard& gb); //결정한 행동 실행
    //void sight(); //현재 위치에서 시야에 있는 오브젝트 확인, 시야 개념은 복잡하므로 초기 버전에선 삭제
};

void fight(chromosome& ch1, chromosome& ch2); //두 개체를 전투 상태로 만듦
#endif