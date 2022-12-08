#ifndef __BOARD_HH__
#define __BOARD_HH__

#include <vector>
#include <stdint.h>
#include <algorithm>

using namespace std;

class BoardEncoding {
    public:
        BoardEncoding();
        BoardEncoding(uint64_t value, int size);
        int get_value_at(int index) { return encoding[index]; }
        void set_value_at(int index, int value) {encoding[index] = value;}
        int get_open_space();
        uint64_t value(); 
    private:
        //first item is msb
        vector<int> encoding;
        vector<int> open_spaces;
};

class Board {
    public:
        Board(int dim);
        Board(BoardEncoding enc, int dim);
        void print();
        int get_value_at(int index) {return encoding.get_value_at(index);}
        void set_value_at(int index, int value) {encoding.set_value_at(index, value);}
        int get_dimension() {return dimension;}
        int get_num_squares() {return dimension * dimension;}
        BoardEncoding get_encoding() {return encoding;}
    private:
        BoardEncoding encoding;
        int dimension;
};

class BoardEncodings {
    public:
        BoardEncodings(int dim);
        size_t size() {return encodings.size();}
        BoardEncoding get(int index) {return encodings[index];}
    private:
        vector<BoardEncoding> encodings;
};

#endif
