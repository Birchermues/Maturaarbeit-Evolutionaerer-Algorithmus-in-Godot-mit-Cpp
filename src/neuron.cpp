#include "neuron.h"

//#include <algorithm>


Neuron::Neuron(uint16_t layer_id, uint16_t neuron_id) 
: layer_id{layer_id}, neuron_id{neuron_id}
{}

void Neuron::calc_value() {
    value = 0.0;

    for (const Connection& connection : connections)
    {
        value += connection.weight * connection.neuron.value + connection.bias;
    }
    value = activation_function(value);
}



float Neuron::activation_function(float input) {
    return input / (1.0f + std::abs(input));
}

