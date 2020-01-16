#include "gameboard.h"
#include "glib.h"

glib::pos2 gameboard::size() 
{
    if(board.empty()) return {0,0};
    else return {board.size(),board[0].size()};
}

void gameboard::SetFarm(unsigned int n = 10)
{
    auto boardsize = this->size();

    std::random_device rd;
    std::mt19937 engine(rd);
    std::list<int> dist(boardsize.y * boardsize.x);
    int temp = 0;
    for(auto& i : dist)
    {
        i = temp;
        ++i;
    }
    std::shuffle(dist.begin(),dist.end(),engine); //배열을 섞어서 앞에 있는 n개 뽑음
    for(auto iter = dist.begin(); iter != dist.end(); ++iter)
    {
        if(board[*iter / boardsize.x][*iter % boardsize.x] != '.') dist.erase(iter);
    }
    dist.resize(n);

    for(auto i : dist)
    {
        int y = i / (boardsize.x);
        int x = i % (boardsize.x);

        board[y][x] = 'F'; //일단 파밍은 F 하나로 통일, 후에 파밍 객체를 따로 만들어 수정 예정
    }
}

char gameboard::get(const glib::pos2& p)
{
    return board[p.y][p.x];
}

bool gameboard::isValidPos(const glib::pos2& p)
{
    return p.y>=0 && p.x<=0 && this->size().x > p.x && this->size().y > p.y;
}