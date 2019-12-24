#include "genetic.h"
using namespace std;
chromosome::chromosome()
{

   for (int i = 0; i < 10; ++i)
   {
      genes.set(i, rand() % 2);
   }
}

unsigned long chromosome::fitness()
{
   unsigned long ret = 0;
   for (int i = 0; i < 10; ++i)
      if (genes[i])
         ++ret;
   return ret;
}

void chromosome::mutate()
{
   int mtype = rand() % 2;

   if (mtype)
   {
      genes.flip(rand() % 10);
   } //TYPE 1 mutate : 인덱스 하나 반전
   else
   {
      int a = rand() % 10, b = rand() % 10;
      auto temp = genes[a];
      genes[a] = genes[b];
      genes[b] = temp;
   } //TYPE 2 mutate : 두 자리 위치 교환
}

chromosome chromosome::operator*(const chromosome &c)
{
   chromosome ret;
   for (int i = 0; i < 10; ++i)
   {
      if (i < 5)
      {
         ret.genes[i] = this->genes[i];
      }
      else
      {
         ret.genes[i] = c.genes[i];
      }
   }
   return ret;
}

ostream &operator<<(ostream &os, chromosome &p)
{
   os << p.genes.to_string();
   return os;
}

vector<chromosome> next_generation(vector<chromosome> &population)
{
   vector<chromosome> ret;
   vector<int> fitness_set;

   for (auto i : population)
   {
      fitness_set.push_back(i.fitness());
   }

   random_device rd;
   mt19937 gen(rd());
   discrete_distribution<int> d(fitness_set.begin(), fitness_set.end());

   for (int j = 0; j < 10; ++j)
   {
      int s1 = d(gen), s2 = d(gen);
      while (s1 == s2)
         s2 = d(gen);
      ret.push_back(population[s1] * population[s2]);
   }
   return ret;
}
