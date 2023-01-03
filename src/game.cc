#include <iostream>
#include "game.hh"

TTTGame::TTTGame(TTTPlayer *p1, TTTPlayer *p2, int dim) : player1_board(dim), player2_board(dim) {
    player1 = p1;
    player2 = p2;

    int num_win_positions = 2 * (dim + 1);
    win_positions = vector<vector<int> >(num_win_positions);

    for (int i = 0; i < dim; i++) {
        //diagonals:
        win_positions[dim * 2].push_back(i * dim + i);
        win_positions[dim * 2 + 1].push_back(i * dim + (dim - i - 1));
        for (int j = 0; j < dim; j++) {     
            //rows:
            win_positions[i].push_back(i * dim + j);
            //cols:
            win_positions[dim + i].push_back(j * dim + i);
        }
    }
}

TTTPlayer *TTTGame::play(bool print_board) {
    TTTPlayer *winner;
    int num_moves = 0;
    while ((winner = get_winner()) == nullptr && num_moves < player1_board.get_num_squares()) {
        if (print_board) player1_board.print();
        TTTPlayer *curr = num_moves % 2 ? player2 : player1;
        do_turn(curr);
        num_moves++;
    }
    if (print_board) player1_board.print();

    if (winner == nullptr) {
        player1->add_to_fitness(-7);
        player2->add_to_fitness(-7);
        if (print_board) printf("Result: Tie");
    } else {
        int dim = player1_board.get_dimension();
        int area = dim * dim;
        TTTPlayer *loser = winner == player1 ? player2 : player1;
        if (num_moves < num_moves + 2) {
            winner->add_to_fitness(50);
            loser->add_to_fitness(-50);
        } else if (num_moves > 3 * area / 4) {
            winner->add_to_fitness(6);
            loser->add_to_fitness(-3);
        } else {
            winner->add_to_fitness(7);
            loser->add_to_fitness(-3);
        }
        if (print_board) printf("Result: TTTPlayer %d", winner == player1 ? 1 : 2);
    }
    return winner;
}

TTTPlayer *TTTGame::winner_at(vector<int> win_position) {
    int curr_val = 0;
    for (int i = 0; i < win_position.size(); i++) {
        int new_val = player1_board.get_value_at(win_position[i]);
        if (new_val == 0) return nullptr;

        if (new_val != curr_val) {
            if (curr_val == 0) {
                curr_val = new_val;
            } else return nullptr;
        }
    }
    if (curr_val == 0) return nullptr;
    return curr_val == 1 ? player1 : player2;
}

TTTPlayer *TTTGame::get_winner() {
    TTTPlayer *winner = nullptr;
    for (int i = 0; i < win_positions.size(); i++) {
        winner = winner_at(win_positions[i]);
        if (winner != nullptr) break;
    }
    return winner;
}

void TTTGame::do_turn(TTTPlayer *player) {
    if (player == player1) {
        Gene gene = player1->do_turn(&player1_board);
        player1_board.set_value_at(gene, 1);
        player2_board.set_value_at(gene, 2);
    } else {
        Gene gene = player2->do_turn(&player2_board);
        player2_board.set_value_at(gene, 1);
        player1_board.set_value_at(gene, 2);
    }
}

void TTTGame::print_board() {
    player1_board.print();
}