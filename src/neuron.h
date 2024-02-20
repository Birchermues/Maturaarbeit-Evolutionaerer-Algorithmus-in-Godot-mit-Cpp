#pragma once

#include <vector>

class Neuron;

class Connection {
    public:
        float weight = 0;
        Neuron& neuron;
};


class Neuron {
    public:
        Neuron(uint16_t layer_id, uint16_t neuron_id);

        float value = 0.0f;

        uint16_t layer_id;
        uint16_t neuron_id;

        void calc_value();

        std::vector<Connection> connections;

        float activation_function(float input);
};
