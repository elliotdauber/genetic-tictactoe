#include "tttgenmod.hh"

int main() {
    int board_dim = 4;
    int num_players = 10;
    int num_rounds_per_gen = 20;
    int num_gens = 20;
    TTTGeneticModeler modeler{num_players, num_rounds_per_gen, num_gens, board_dim};
    modeler.run();
}