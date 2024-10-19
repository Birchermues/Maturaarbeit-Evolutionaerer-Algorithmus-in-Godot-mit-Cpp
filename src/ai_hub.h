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
            float mut_strength = 0.1f;


            String ai_player_group_name;

            void sort_nns_on_score();

            TypedArray<nn> nns;


            // unbenutzt
            int get_generation() const { return generation; }
            void set_generation(int generation_) { generation = generation_; }

            // unbenutzt
            float get_best_score() const { return best_score; }
            void set_best_score(float bestScore) { best_score = bestScore; }

            // unbenutzt
            bool get_train_ai() const { return train_ai; }
            void set_train_ai(bool trainAi) { train_ai = trainAi; }

            // unbenutzt
            int get_num_players() const { return num_players; }
            void set_num_players(int numPlayers) { num_players = numPlayers; }

            // chance mit der ein float wert mutiert
            float get_mut_chance() const { return mut_chance; }
            void set_mut_chance(float mutChance) { mut_chance = mutChance; }

            // stärke der mutation
            float get_mut_strength() const { return mut_strength; }
            void set_mut_strength(float MutStrength) { mut_strength = MutStrength; }


            String get_ai_player_group_name() const { return ai_player_group_name; }
            void set_ai_player_group_name(const String& groupName) { ai_player_group_name = groupName; }

            // die neuronalen netze
            TypedArray<nn> get_nns() { return nns; }
            void set_nns(const TypedArray<nn>& nns_) { nns = nns_; }

            // unbenutzt
            void inherit();

    };
}
