#ifndef __GAMEBASE_HH__
#define __GAMEBASE_HH__

#include "playerbase.hh"

using namespace std;

class GameBase {
    public:
        virtual PlayerBase *play(bool print_board) = 0;
        virtual ~GameBase() = default;
};

#endif