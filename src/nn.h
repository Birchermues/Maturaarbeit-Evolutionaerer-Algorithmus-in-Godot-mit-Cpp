#pragma once

#include <godot_cpp/classes/node.hpp>
#include "neuron.h"
#include "layer.h"

#include <vector>
#include <compare>
#include <iostream>
#include <set>
//#include <godot_cpp\classes\resource.hpp>

namespace godot {
    class nn : public Node {
        GDCLASS(nn, Node);
        private:
            std::vector<float> weights_and_biases;
        
        protected:
            static void _bind_methods();

        public:

            // NEURONS, LAYERS, CONNECTIONS, ETC.

            // nn();

            //void set_network(const Resource &_network_res) {network = _network_res; }
            
            void set_layers(TypedArray<int> layer_layout);
            TypedArray<int> get_layers() const;

            std::vector<Layer> layers;

            void fill_connections();

            std::vector<std::byte> serialize() const;
            std::vector<float> float_serialize(const std::vector<Layer>& layers) const;

            void deserialize(const std::vector<std::byte>& binary);
            void float_deserialize(std::vector<float> &weights_and_biases);

            void set_weights_and_biases(godot::TypedArray<float> weights_and_biases);
            godot::TypedArray<float> get_weights_and_biases() const;


            //SOLVING, FORWARD PROPAGATION

            godot::TypedArray<float> solve(godot::TypedArray<float> Inputs);


            //MUTATION, MIXING UP THE GENES, RANDOMNESS
            void mutate(float mut_chance, float weight_mut_strength, float bias_mut_strength);

            void randomize_weights_and_biases(bool use_normal_distribution, float max_weight, float max_bias);

            std::partial_ordering operator<=>(const nn& other) const;

            bool operator<(const nn& other) const;
            

            //TypedArray<float> scores;
            std::vector<float> scores;
            float score {0};

            void set_score(float score_) { score = score_; }
            float get_score() const { return score; }

            //float calc_score();
            
            //void reset_score();
    };
}

std::vector<Layer> create_layers(std::vector<int> layer_layout);
