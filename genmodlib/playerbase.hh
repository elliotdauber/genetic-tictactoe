#ifndef __PLAYERBASE_HH__
#define __PLAYERBASE_HH__

using namespace std;

class PlayerBase {
    public:
        PlayerBase() : fitness(0) {}
        PlayerBase(int fit) : fitness(fit) {}
        virtual void add_to_fitness(int fit) = 0;
        virtual int get_fitness() = 0;
        virtual ~PlayerBase() = default;
    protected:
        int fitness;
};  

#endif