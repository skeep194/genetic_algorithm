class chromosome
{
private:
   std::bitset<10> genes;

public:
   chromosome();
   unsigned long fitness();
   chromosome operator*(const chromosome &c);
   friend ostream &operator<<(ostream &os, chromosome &p);
   void mutate();
};
