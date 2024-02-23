#pragma once
#include "nn.h"

#include <godot_cpp/classes/node.hpp>


namespace godot {
    class ai_hub: public Node {
        GDCLASS(ai_hub, Node);
        
        protected:
            static void _bind_methods();
        public:
            ai_hub();

            int generation = 0;
            
            float best_score = 0;

            bool train_ai = true;

            int num_players = 1;

            float mut_chance = 0.1f;
            float weight_mut_strength = 0.1f;
            float bias_mut_strength = 0.1f;

            void sort_nns_on_score();

            TypedArray<nn> nns;

            bool compare_nn();

            int get_generation() const { return generation; }
            void set_generation(int generation_) { generation = generation_; }

            float get_best_score() const { return best_score; }
            void set_best_score(float bestScore) { best_score = bestScore; }

            bool get_train_ai() const { return train_ai; }
            void set_train_ai(bool trainAi) { train_ai = trainAi; }

            int get_num_players() const { return num_players; }
            void set_num_players(int numPlayers) { num_players = numPlayers; }

            float get_mut_chance() const { return mut_chance; }
            void set_mut_chance(float mutChance) { mut_chance = mutChance; }

            float get_weight_mut_strength() const { return weight_mut_strength; }
            void set_weight_mut_strength(float weightMutStrength) { weight_mut_strength = weightMutStrength; }

            float get_bias_mut_strength() const { return bias_mut_strength; }
            void set_bias_mut_strength(float biasMutStrength) { bias_mut_strength = biasMutStrength; }

            

            
    };
}
