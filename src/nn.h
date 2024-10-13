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

            // Gewichte und Biases welche seriell gespeichert werden
            std::vector<float> weights_and_biases;
        
        protected:
            static void _bind_methods();

        public:

            // enthaltenen Schichten
            std::vector<Layer> layers;

            // Schichtengrösse bzw. Anzahl der Neuronen pro Schicht
            void set_layers(TypedArray<int> layer_layout);
            TypedArray<int> get_layers() const;

            // Verbindungen zwischen den Neuronen korrekt initialisieren
            void fill_connections();

            // Gewichte und Biases serialisieren und deserialisieren
            std::vector<std::byte> serialize() const;
            std::vector<float> float_serialize(const std::vector<Layer>& layers) const;
            void float_deserialize(std::vector<float> &weights_and_biases);

            // Gewichte und Biases setzen und abrufen
            void set_weights_and_biases(godot::TypedArray<float> weights_and_biases);
            godot::TypedArray<float> get_weights_and_biases() const;


            // vorwärtsschritt (berechnung des outputs mit gegebenem input)
            godot::TypedArray<float> solve(godot::TypedArray<float> Inputs);


            // mutation
            void mutate(float mut_chance, float mut_strength);

            // zufällige initialisierung der Gewichte und Biases
            void randomize_weights_and_biases(bool use_normal_distribution, float max_weight, float max_bias);


            // score des spielers mit diesem neuronalen netz
            std::partial_ordering operator<=>(const nn& other) const;
            bool operator<(const nn& other) const;
            float score {0};
            void set_score(float score_) { score = score_; }
            float get_score() const { return score; }
    };
}

std::vector<Layer> create_layers(std::vector<int> layer_layout);
