#include "tttgenmod.hh"

int main() {
    int board_dim = 5;
    int num_players = 40;
    int num_rounds_per_gen = 100;
    int num_gens = 100;
    TTTGeneticModeler modeler{num_players, num_rounds_per_gen, num_gens, board_dim};
    modeler.run();
}