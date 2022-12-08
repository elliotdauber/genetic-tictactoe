#ifndef __PLAYER_HH__
#define __PLAYER_HH__

#include <map>
#include <vector>
#include <string>
#include "playerbase.hh"
#include "board.hh"

// #define BOARD_DIMENSION 4
// #define NUM_SQUARES BOARD_DIMENSION * BOARD_DIMENSION

using namespace std;

typedef short Gene;

class TTTPlayer : public PlayerBase {
    public:
        TTTPlayer() : PlayerBase() {}
        virtual Gene do_turn(Board *board) = 0;
        int get_fitness() override {return fitness;}
        void add_to_fitness(int fit) override {fitness += fit;}
        // virtual ~TTTPlayer() = 0;
};

class TTTModelPlayer : public TTTPlayer {
    public:
        Gene do_turn(Board *board) {return gene_for(board->get_encoding());}
        TTTModelPlayer(BoardEncodings *encodings);
        TTTModelPlayer(TTTModelPlayer *mother, TTTModelPlayer *father);
        TTTModelPlayer(string filename);
        Gene gene_for(BoardEncoding encoding);
        Gene gene_for(uint64_t value);
        void write_genome_to_file();
    private:
        vector<Gene> genes;
};

class TTTHumanPlayer : public TTTPlayer {
    public:
        TTTHumanPlayer() : TTTPlayer() {}
        Gene do_turn(Board *board) {
            cout << "Input> ";
            Gene input;
            cin >> input;
            return input;
        }
};

#endif
