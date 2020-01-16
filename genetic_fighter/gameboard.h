#ifndef GAMEBOARD_H
#define GAMEBOARD_H
/*
실제 n * m 크기의 격자 지형을 생성
. - 이동 가능한 구역
# - 이동 불가능한 구역
숫자, 문자 (a - z) - 식별번호( 개체 혹은 파밍 스팟 )
*/
#include <vector>
#include <random>
#include <algorithm>
#include <list>

class gameboard
{
private:
    std::vector<std::vector<char>> board;
public:
    gameboard();
    gameboard(glib::pos2 p)
    {
        board.assign(p.y,std::vector<char>(p.x,'.'));
    }
    char get(const glib::pos2& p);
    glib::pos2 size(); //(y, x) 사이즈 리턴
    void SetFarm(unsigned int n = 10); //랜덤한 구역에 파밍 스팟 생성 : default 10개
    bool isValidPos(const glib::pos2& p);
};

#endif