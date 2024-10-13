#include "neuron.h"

//#include <algorithm>


Neuron::Neuron(uint16_t layer_id, uint16_t neuron_id) 
: layer_id{layer_id}, neuron_id{neuron_id}
{}

void Neuron::calc_value() {
    // die Werte der vorherigen Neuronen mit den Gewichten multiplizieren und summieren
    value = 0.0;
    for (const Connection& connection : connections)
    {
        value += (connection.weight * connection.neuron.value);
    }
    // bias addieren und dann die Aktivierungsfunktion anwenden
    value = activation_function(value + bias);
}

float Neuron::activation_function(float input) {
    if (input > 0) {return input; }     //<---- leaky relu
    return input * 0.2f;                //<---- leaky relu
}

