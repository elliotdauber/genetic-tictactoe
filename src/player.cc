#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <random>
#include <cmath>
#include <map>
#include <fstream>
#include <algorithm>
#include <sys/stat.h> // for mkdir on Linux and macOS
#include "player.hh"


TTTModelPlayer::TTTModelPlayer() : TTTPlayer() {}

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
    // Gene gene;
    string line;
    getline(file, line); //dimension line
    while (getline(file, line)) {
        stringstream ss(line);
        int key, value;
        char separator;
        ss >> key >> separator >> value;
        gene_map.insert({key, value});
    }
    // while (file >> gene) {
    //    genes.push_back(gene);
    // }
}

TTTModelPlayer::TTTModelPlayer(TTTModelPlayer *mother, TTTModelPlayer *father) : TTTPlayer((mother->get_fitness() + father->get_fitness()) / 2) {
    // uint64_t num_genes = mother->genes.size();
    // uint64_t gene_to_split_at = rand() % num_genes;
    // for (uint64_t i = 0; i < gene_to_split_at; i++) {
    //     Gene g = mother->gene_for(i);
    //     genes.push_back(g);
    // }

    // for (uint64_t i = gene_to_split_at; i < num_genes; i++) {
    //     Gene g = father->gene_for(i);
    //     genes.push_back(g);
    // }

    //TODO: BAD STYLE
  size_t smallest = min(mother->gene_map.size(), father->gene_map.size());
  int gene_to_split_at = rand() % smallest;
  auto mother_elem = next(mother->gene_map.begin(), gene_to_split_at);
  auto father_elem = next(father->gene_map.begin(), gene_to_split_at);
  for (auto elem = mother->gene_map.begin(); elem != mother_elem; elem++) {
    gene_map.insert(*elem);
  }
  for (auto elem = father_elem; elem != father->gene_map.end(); elem++) {
    gene_map.insert(*elem);
  }
}

// Gene TTTModelPlayer::gene_for(BoardEncoding encoding) {
//     return genes[encoding.value()];
// }

// Gene TTTModelPlayer::gene_for(uint64_t value) {
//     return genes[value];
// }

void TTTModelPlayer::write_genome_to_file(int dim) {
    mkdir("saves", 0777);

    auto t = time(nullptr);
    auto tm = *localtime(&t);

    ostringstream oss;
    oss << "save_" << put_time(&tm, "%d-%m-%Y--%H-%M-%S") << ".ttt";
    auto filename = "saves/" + oss.str();

    ofstream file(filename);
    printf("writing to %s...\n", filename.c_str());
    // for (size_t i = 0; i < genes.size(); i++) {
    //     file << genes[i] << endl;
    // }
    file << dim << endl;
    for (const auto& elem : gene_map) {
        // Write the key and value to the file in the form "key:value"
        file << elem.first << ":" << elem.second << endl;
    }
    file.close();
}
