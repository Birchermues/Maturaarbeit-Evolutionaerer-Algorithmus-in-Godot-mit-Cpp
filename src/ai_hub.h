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

            String ai_player_group_name;

            void sort_nns_on_score();

            TypedArray<nn> nns;

            // TypedArray<nn> convert_vector_to_typed_array(const std::vector<nn>& vector) {
            //     TypedArray<nn> array;
            //     array.resize(vector.size());
            //     for (size_t i = 0; i < vector.size(); i++) {
            //         array.push_back(&vector.at(i));
            //     }
            //     return array;
            // }

            // std::vector<nn> convert_typed_array_to_vector(const TypedArray<nn>& array) {
            //     std::vector<nn> vector;
            //     for (size_t i = 0; i < array.size(); i++) {
            //         nn nn_ = VariantCaster<nn>::cast(array[i]);
            //         vector.push_back(nn_);
            //     }
            //     return vector;
            // }

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

            String get_ai_player_group_name() const { return ai_player_group_name; }
            void set_ai_player_group_name(const String& groupName) { ai_player_group_name = groupName; }

            TypedArray<nn> get_nns() { return nns; }
            void set_nns(const TypedArray<nn>& nns_) { nns = nns_; }
            //void add_nn(const nn& nn_) { nns.append()); }

            //bool custom_sort_func(const nn& a, const nn& b) { return a.get_score() < b.get_score();}
    };
}
