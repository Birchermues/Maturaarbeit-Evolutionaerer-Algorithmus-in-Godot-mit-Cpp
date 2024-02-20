#include "layer.h"
//#include <cmath>


Layer::Layer(int num_neurons, uint16_t layer_id)
    : layer_id{layer_id} {
        for (uint16_t id = 0; id < num_neurons; id++) {
            neurons.push_back(Neuron(layer_id, id));
        }
    }



void Layer::calc_values() {
    for (Neuron& neuron : neurons) {
        neuron.calc_value();
    }
}