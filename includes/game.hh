#ifndef __GAME_HH__
#define __GAME_HH__

#include <vector>
#include <random>
#include "gamebase.hh"
#include "player.hh"
#include "board.hh"

using namespace std;

class TTTGame : GameBase {
    public:
        TTTGame(TTTPlayer *p1, TTTPlayer *p2, int dim);
        TTTPlayer *play(bool print_board) override; //null = draw
        void do_turn(TTTPlayer *player);
        void print_board();
        TTTPlayer *get_winner();
        TTTPlayer *winner_at(vector<int> win_position);
    private:
        TTTPlayer *player1;
        TTTPlayer *player2;
        Board player1_board;
        Board player2_board;
        vector<vector<int> > win_positions;
};

#endif
