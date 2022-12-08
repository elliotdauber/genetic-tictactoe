#include <iostream>
#include <cmath>
#include <omp.h>
#include "board.hh"

Board::Board(int dim) : encoding(0, dim * dim), dimension(dim) {}

void Board::print() {
    cout << endl;
    for (int i = 0; i < dimension; i++) {
        cout << "----";
    }
    cout << "-" << endl;

    for (int row = 0; row < dimension; row++) {
        cout << "|";
        for (int col = 0; col < dimension; col++) {
            int index = (row * dimension) + col;
            int val = get_value_at(index);
            if (val == 0) {
                cout << "   |";
            } else if (val == 1) {
                cout << " x |";
            } else if (val == 2) {
                cout << " o |";
            } else {
                cout << " " << "?" << " |";
            }
        }   
        cout << endl;
        for (int i = 0; i < dimension; i++) {
            cout << "----";
        }
        cout << "-" << endl;
    }
    cout << endl;
}

BoardEncoding::BoardEncoding(uint64_t value, int size) {
    int i = 0;
    while (value > 0 || i < size) {
        int cur_val = value % 3;
        if (cur_val == 0) {
            open_spaces.push_back(i);
        }
        encoding.push_back(cur_val);
        value /= 3;
        i++;
    }

    reverse(encoding.begin(), encoding.end());
}

BoardEncoding::BoardEncoding() {}

uint64_t BoardEncoding::value() {
    uint64_t value = 0;
    int base = 1;
    for (int i = 0; i < encoding.size(); i++) {
        value += encoding[i] * base;
        base *= 3;
    }
    return value;
}

int BoardEncoding::get_open_space() {
    if (open_spaces.size() == 0) {
        return 0; //TODO: -1? this shouldn't happen
    }
    int index = rand() % open_spaces.size();
    return open_spaces[index];
}

BoardEncodings::BoardEncodings(int dim) {
    uint64_t num_encodings = pow(3, dim*dim);
    printf("num_encodings: %llu\n", num_encodings);

    encodings = vector<BoardEncoding>(num_encodings);
    
    #pragma omp parallel for schedule(dynamic, 1000)
    for (uint64_t i = 0; i < num_encodings; i++) {
        encodings[i] = BoardEncoding(i, dim * dim);
    }
}
