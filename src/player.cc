#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <random>
#include <cmath>
#include <map>
#include <fstream>
#include <sys/stat.h> // for mkdir on Linux and macOS
#include "player.hh"

//todo: can't do any random number -- beteen 0 and 8, because there are only some free spaces
//idea: have to loop through all posibilities, and create map from posibility nunber : free spaces
//use number to encode layout: what does the number represent??

TTTModelPlayer::TTTModelPlayer(BoardEncodings *encodings) : TTTPlayer() {
    for (int i = 0; i < encodings->size(); i++) {
        BoardEncoding encoding = encodings->get(i);
        Gene open_space = encoding.get_open_space();
        genes.push_back(open_space);
    }
}

TTTModelPlayer::TTTModelPlayer(string filename) : TTTPlayer() {
    ifstream file(filename.c_str());
    Gene gene;
    while (file >> gene) {
       genes.push_back(gene);
    }
}

TTTModelPlayer::TTTModelPlayer(TTTModelPlayer *mother, TTTModelPlayer *father) : TTTPlayer() {
    uint64_t num_genes = mother->genes.size();
    uint64_t gene_to_split_at = rand() % num_genes;
    for (uint64_t i = 0; i < gene_to_split_at; i++) {
        Gene g = mother->gene_for(i);
        genes.push_back(g);
    }

    for (uint64_t i = gene_to_split_at; i < num_genes; i++) {
        Gene g = father->gene_for(i);
        genes.push_back(g);
    }
}

Gene TTTModelPlayer::gene_for(BoardEncoding encoding) {
    return genes[encoding.value()];
}

Gene TTTModelPlayer::gene_for(uint64_t value) {
    return genes[value];
}

void TTTModelPlayer::write_genome_to_file() {
    mkdir("saves", 0777);

    auto t = time(nullptr);
    auto tm = *localtime(&t);

    ostringstream oss;
    oss << "save_" << put_time(&tm, "%d-%m-%Y--%H-%M-%S") << ".ttt";
    auto filename = "saves/" + oss.str();

    ofstream file(filename);
    printf("writing to %s...\n", filename.c_str());
    for (size_t i = 0; i < genes.size(); i++) {
        file << genes[i] << endl;
    }
    file.close();
}
