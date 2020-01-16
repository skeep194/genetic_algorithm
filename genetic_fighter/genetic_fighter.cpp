#include "gameboard.h"
#include "genetic_fighter.h"
#include "glib.h"



chromosome::chromosome(int _uniq_num) : uniq_num(_uniq_num)
{
    engine.seed(rd());

    HP = 1000;
    int statsum = 3000;

    std::vector<int>* s = new std::vector<int>(3);
    for(int i=0;i<2;++i)
    {
        int now = engine()%statsum;
        statsum -= now;
        (*s)[i] = now;
    }
    (*s)[2] = statsum;
    stat = preference = glib::status((*s)[0],(*s)[1],(*s)[2]);
    delete s;

    std::uniform_int_distribution<> dist(1,100);
    for(int i=0;i<3;++i)
    {
        tendency[i] = dist(engine);
    }

    prev = -1;
}

void chromosome::turn_act() //이번 턴에 해야 할 행동을 결정함 (나중에 ai는 계속 수정할 예정)
{
    std::uniform_int_distribution<> dist(1,100);
    if(!isFight.empty())
    {
        if(prev != 1 && prev != 2) //저번턴에 한 행동이 전투가 아님
        {
            if(tendency[0] <= dist(engine)) //전투 받아들임, 자신과 전투적인 적 중 가장 가까운 적을 향해 이동
            {
                prev = 1;
            }
            else //도망침, 자신과 전투적인 적 중 가장 가까운 적을 피해 이동
            {
                prev = 2;
            }
        }
        else
        {
            if(tendency[2] <= dist(engine)) //행동 바꾸는 성향 체크
            {
                if(prev == 1) prev = 2;
                else prev = 1;
            }
        }
        
    }
    else
    {
        if(prev == -1 || tendency[2] <= dist(engine)) //새로운 행동을 갱신해야 하는 경우, 아닐 시 기존 행동을 반복하면 됨
        {
            if(tendency[0] <= dist(engine))
            {
                prev = 3;
            }
            else
            {
                prev = 4;
            }
        }
    }
}

void chromosome::turn(const gameboard& gb) //갱신된 prev를 이용해 실제 턴 진행
{
    switch(prev)
    {
        case 1:
        case 2:
        case 3:
        case 4:
        default:
            throw "EXCEPT_NO_PREV";
    }
}

unsigned int chromosome::target(gameboard& gb, std::set<glib::pos2> evade = std::set<glib::pos2>())
{
    const int dir[4][2] = {{1,0},{0,1},{-1,0},{0,-1}};
    std::queue<glib::pos2> q;
    q.push(pos);
    unsigned int depth = 0;

    bool isEnemy = prev != 4; //true일 시 적을 찾음, false일 시 파밍스팟을 찾음

    std::vector<std::vector<glib::pos2>> prevroute; //전에 간 경로 저장
    prevroute.assign(gb.size().y, std::vector<glib::pos2>(gb.size().x));

    while(!q.empty())
    {
        ++depth;
        int qsize = q.size();
        for(int i=0;i<qsize;++i)
        {
            auto now = q.front();
            q.pop();
            for(int j=0;j<4;++j)
            {
                glib::pos2 nextp(now.y+dir[j][0], now.x+dir[j][1]);
                if(gb.isValidPos(nextp))
                {
                    if(gb.get(nextp) == '#' || prevroute[nextp.y][nextp.x].isValid() || evade.find(nextp) != evade.end()) continue;
                    else
                    {
                        prevroute[nextp.y][nextp.x] = now;
                        char object = gb.get(nextp);
                        if(isEnemy && '0' <= object && object <= '9' && isFight.find(object - '0') != isFight.end()) //TODO: gb에 나타나는 적 식별 정수형으로 가능하게 하기
                        {
                            glib::pos2 back = nextp;
                            while(back.isValid())
                            {
                                route.push_back(back);
                                back = prevroute[back.y][back.x];
                            }
                            return depth; //적 찾음
                        } 
                        else if(!isEnemy && object == 'F')
                        {
                            glib::pos2 back = nextp;
                            while(back.isValid())
                            {
                                route.push_back(back);
                                back = prevroute[back.y][back.x];
                            }
                            return depth; //파밍 찾음
                        }
                    }
                }
            }
        }
    }
    return 0; //갈 방법 없음
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