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
        TTTPlayer(int fit) : PlayerBase(fit) {}
        virtual Gene do_turn(Board *board) = 0;
        int get_fitness() override {return fitness;}
        void add_to_fitness(int fit) override {fitness += fit;}
};

class TTTModelPlayer : public TTTPlayer {
    public:
        // Gene do_turn(Board *board) {return gene_for(board->get_encoding());}
        Gene do_turn(Board *board) {
            BoardEncoding encoding = board->get_encoding();
            auto it = gene_map.find(encoding.value());
            if (it == gene_map.end()) {
                Gene gene = encoding.get_open_space();
                gene_map.insert({encoding.value(), gene});
                return gene;
            }
            return it->second;
        }
        TTTModelPlayer();
        TTTModelPlayer(BoardEncodings *encodings);
        TTTModelPlayer(TTTModelPlayer *mother, TTTModelPlayer *father);
        TTTModelPlayer(string filename);
        void write_genome_to_file(int dim);
        map<uint64_t, Gene> gene_map; //TODO: BAD STYLE
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
