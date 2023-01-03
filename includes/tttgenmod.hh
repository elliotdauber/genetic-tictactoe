#ifndef __TTTGENMOD_H__
#define __TTTGENMOD_H__

#include "genmodbase.hh"
#include "game.hh"

class TTTGeneticModeler : public GeneticModelerBase<TTTModelPlayer, TTTGame> {
    public:
        TTTGeneticModeler(int generation_size, int num_rounds_per_generation, int num_generations, int board_dimension)
            : GeneticModelerBase<TTTModelPlayer, TTTGame>(generation_size, num_rounds_per_generation, num_generations), board_dimension(board_dimension) {}
    private:
        // BoardEncodings *encodings;
        int board_dimension;

        void init() override {
            // encodings = new BoardEncodings(board_dimension);
        }
        void cleanup() override {
            // delete encodings;
        }

        TTTModelPlayer *generate_random_agent() override {
            // return new TTTModelPlayer(encodings);
            return new TTTModelPlayer();
        }
        TTTModelPlayer *generate_child_agent(TTTModelPlayer *mother, TTTModelPlayer *father) override {
            return new TTTModelPlayer(mother, father);
        }
        void delete_agent(TTTModelPlayer *agent) override {
            delete agent;
        }
        int get_agent_fitness(TTTModelPlayer *agent) override {
            return agent->get_fitness();
        }

        void play_game(TTTModelPlayer *agent1, TTTModelPlayer *agent2) override {
            TTTGame game = TTTGame(agent1, agent2, board_dimension);
            game.play(false);
        }

        bool compare_agents(TTTModelPlayer *agent1, TTTModelPlayer *agent2) override {
            return agent1->get_fitness() >= agent2->get_fitness();
        }
};
#endif