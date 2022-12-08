#ifndef __GENMOD_HH__
#define __GENMOD_HH__

#include <vector>
#include <iostream>

using namespace std;

template <typename PlayerT, typename GameT>
class GeneticModelerBase {
    // static_assert(is_base_of<PlayerBase, PlayerT>::value, "PlayerT must inherit from PlayerBase");
    // static_assert(is_base_of<GameBase, GameT>::value, "GameT must inherit from GameBase");
    public:
        GeneticModelerBase(int generation_size, int num_rounds_per_generation, int num_generations);
        virtual ~GeneticModelerBase() = default;
        void run();

    private:
        int generation_size;
        int num_rounds_per_generation;
        int num_generations;

        PlayerT *get_random(vector<PlayerT*> *choices);
        vector<pair<PlayerT*, PlayerT*> > make_pairings(vector<PlayerT*> *choices);
        void run_matches(vector<PlayerT*> *generation);
        void fill_weighted_generation(vector<PlayerT*> *generation, vector<PlayerT*> *weighted_generation);
        void create_new_generation(vector<PlayerT*> *weighted_generation, vector<PlayerT*> *new_generation);
        void run_generation(vector<PlayerT*> *old_generation, vector<PlayerT*> *new_generation);

        virtual void init() = 0;
        virtual void cleanup() = 0;
        virtual PlayerT *generate_random_agent() = 0;
        virtual PlayerT *generate_child_agent(PlayerT *mother, PlayerT *father) = 0;
        virtual void delete_agent(PlayerT *agent) = 0;
        virtual int get_agent_fitness(PlayerT *agent) = 0;
        virtual void play_game(PlayerT *agent1, PlayerT *agent2) = 0; 
};



















//implementation:
//this is in the .hh file becuase of some weird linker error when its in the .cc file

template <typename PlayerT, typename GameT>
GeneticModelerBase<PlayerT, GameT>::GeneticModelerBase(int generation_size, int num_rounds_per_generation, int num_generations) 
                                : generation_size(generation_size), 
                                  num_rounds_per_generation(num_rounds_per_generation),
                                  num_generations(num_generations) {}


template <typename PlayerT, typename GameT>
PlayerT *GeneticModelerBase<PlayerT, GameT>::get_random(vector<PlayerT*> *choices) {
    int index = rand() % choices->size();
    PlayerT *elem = choices->at(index);
    auto it = choices->begin();
    advance(it, index);
    choices->erase(it);
    return elem;
}

template <typename PlayerT, typename GameT>
vector<pair<PlayerT*, PlayerT*> > GeneticModelerBase<PlayerT, GameT>::make_pairings(vector<PlayerT*> *choices) {
    vector<pair<PlayerT*, PlayerT*> > pairings{};
    while (!choices->empty()) {
        PlayerT *choice1 = get_random(choices);
        PlayerT *choice2 = get_random(choices);
        pair<PlayerT*, PlayerT*> pairing = pair<PlayerT*, PlayerT*>(choice1, choice2);
        pairings.push_back(pairing);
    }
    return pairings;
}

template <typename PlayerT, typename GameT>
void GeneticModelerBase<PlayerT, GameT>::run_matches(vector<PlayerT*> *generation) {
    vector<pair<PlayerT*,PlayerT*> > pairings;
    vector<PlayerT*> generation_copy;
    for (int i = 0; i < num_rounds_per_generation; i++) {
        generation_copy = *generation;
        pairings = make_pairings(&generation_copy);

        #pragma omp parallel for
        for (size_t j = 0; j < pairings.size(); j++) {
            play_game(pairings[j].first, pairings[j].second);
        }
    }
}


template <typename PlayerT, typename GameT>
void GeneticModelerBase<PlayerT, GameT>::fill_weighted_generation(vector<PlayerT*> *generation, vector<PlayerT*> *weighted_generation) {
    int min_fitness = get_agent_fitness(generation->at(0));
    for (size_t i = 1; i < generation->size(); i++) {
        PlayerT *player = generation->at(i);
        int fitness = get_agent_fitness(player);
        if (fitness < min_fitness) {
            min_fitness = fitness;
        }
    }
    for (size_t i = 0; i < generation->size(); i++) {
        PlayerT *player = generation->at(i);
        int num_weighted = get_agent_fitness(player) - min_fitness + 1;
        printf("[%zu] num weighted: %d\n", i, num_weighted);
        for (int i = 0; i < num_weighted; i++) {
            weighted_generation->push_back(player);
        }
    }
}

template <typename PlayerT, typename GameT>
void GeneticModelerBase<PlayerT, GameT>::create_new_generation(vector<PlayerT*> *weighted_generation, vector<PlayerT*> *new_generation) {
    size_t num_weighted = weighted_generation->size();
    for (int i = 0; i < generation_size; i++) {
        int mother_idx = rand() % num_weighted;
        int father_idx = rand() % num_weighted;
        PlayerT *mother = weighted_generation->at(mother_idx);
        PlayerT *father = weighted_generation->at(father_idx);
        PlayerT *child = generate_child_agent(mother, father);
        new_generation->push_back(child);
    }
}

template <typename PlayerT, typename GameT>
void GeneticModelerBase<PlayerT, GameT>::run_generation(vector<PlayerT*> *old_generation, vector<PlayerT*> *new_generation) {
    run_matches(old_generation);
    vector<PlayerT*> *weighted_generation = new vector<PlayerT*>;
    fill_weighted_generation(old_generation, weighted_generation);
    create_new_generation(weighted_generation, new_generation);
    
    delete weighted_generation;
}

template <typename PlayerT, typename GameT>
void GeneticModelerBase<PlayerT, GameT>::run() {
    init();

    vector<PlayerT*> *old_generation = new vector<PlayerT*>(generation_size);
    vector<PlayerT*> *new_generation = new vector<PlayerT*>;

    cout << "\tinititalizing " << generation_size << " players... " << endl;

    #pragma omp parallel for
    for (int i = 0; i < generation_size; i++) {
        (*old_generation)[i] = generate_random_agent();
    }

    cout << "done initializing... " << endl;

    for (int i = 0; i < num_generations; i++) {
        run_generation(old_generation, new_generation);
        cout << "ran generation " << i << endl;

        vector<PlayerT*> *tmp = new_generation;
        new_generation = old_generation;
        old_generation = tmp;

        for (int i = 0; i < generation_size; i++) {
            delete_agent(new_generation->at(i));
        }
        new_generation->clear();
    }

    PlayerT *best_player = old_generation->at(0);
    for (int i = 1; i < old_generation->size(); i++) {
        if (get_agent_fitness(old_generation->at(i)) > get_agent_fitness(best_player)) {
            best_player = old_generation->at(i);
        }
    }
    best_player->write_genome_to_file();

    delete old_generation;
    delete new_generation;

    cleanup();
}




#endif