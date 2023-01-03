#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "game.hh"
using namespace std;

void play_game(TTTHumanPlayer *human, TTTModelPlayer *opponent, int dim) {
    TTTGame game = TTTGame(human, opponent, dim); //TODO: get this from the save file!
    game.play(true);
    game.print_board();
}

int main (int argc, char* argv[]) {

    if (argc < 2) {
        cout << "Usage: ./play <filepath>" << std::endl;
        return 1;
    }

    string filepath = argv[1];
    ifstream file(filepath.c_str());
    int dim;
    file >> dim;
    file.close();

    TTTModelPlayer *opponent = new TTTModelPlayer(filepath);
    TTTHumanPlayer *human = new TTTHumanPlayer();
    while (true) {
        play_game(human, opponent, dim);
    }
    delete opponent;
    delete human;
}