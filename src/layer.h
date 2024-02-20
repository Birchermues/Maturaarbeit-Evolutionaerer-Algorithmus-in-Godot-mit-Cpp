#pragma once

#include <vector>
#include "neuron.h"

class Layer {
    public:
        Layer(int num_neurons, uint16_t layer_id);

        std::vector<Neuron> neurons;

        uint16_t layer_id;
        void calc_values();

};