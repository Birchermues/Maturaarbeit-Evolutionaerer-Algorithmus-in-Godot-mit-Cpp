#pragma once

#include <vector>
#include "neuron.h"

class Layer {
    public:
        Layer(int num_neurons, uint16_t layer_id);
        
        // alle Neuronen in dieser Schicht
        std::vector<Neuron> neurons;
        // um welche Schicht handelt es sich
        uint16_t layer_id;
        // berechnet die Werte der Neuronen in dieser Schicht beim vorwärtsschritt
        void calc_values();
};