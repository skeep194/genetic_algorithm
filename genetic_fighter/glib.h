#ifndef GLIB_H
#define GLIB_H

#include <utility>
namespace glib
{
struct pos2
{
    int y, x;
    pos2() : y(-1), x(-1) {}
    pos2(const int& _y, const int& _x) : y(_y), x(_x) {}
    pos2(std::pair<int,int> _p) : y(_p.first), x(_p.second) {} 
    bool isValid()
    {
        return y>=0&&x>=0;
    }
    bool operator==(const pos2& p1) {return y == p1.y && x == p1.x;}
};

struct status
{
    int atk, dfk, avg; //공, 방, 민첩
    status() : atk(1000),dfk(1000),avg(1000) {}
    status(int _atk, int _dfk, int _avg) : atk(_atk), dfk(_dfk), avg(_avg) {}
};
}
#endif