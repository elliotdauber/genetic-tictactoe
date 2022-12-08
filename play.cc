#include <iostream>
#include <map>
#include "game.hh"
using namespace std;

void play_game(TTTHumanPlayer *human, TTTModelPlayer *opponent) {
    TTTGame game = TTTGame(human, opponent, 4);
    game.play(true);
    game.print_board();
}

int main () {
    string filename = "save_bigboy.ttt";
    TTTModelPlayer *opponent = new TTTModelPlayer(filename);
    TTTHumanPlayer *human = new TTTHumanPlayer();
    while (true) {
        play_game(human, opponent);
    }
    delete opponent;
    delete human;
}