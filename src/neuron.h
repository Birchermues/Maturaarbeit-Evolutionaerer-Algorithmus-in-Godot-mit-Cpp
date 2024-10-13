#pragma once

#include <vector>

class Neuron;

// Verbindung mit vorherigem Neuron
class Connection {
    public:
        // Gewicht der Verbindung
        float weight = 0.0f;
        
        // referenz zum vorherigen Neuron
        Neuron& neuron;
};

class Neuron {
    public:
        Neuron(uint16_t layer_id, uint16_t neuron_id);
        // Wert des Neurons
        float value = 0.0f;

        // Bias des Neurons
        float bias = 0.0f;

        // um welches Neuron/welche Schicht handelt es sich
        uint16_t layer_id;
        uint16_t neuron_id;

        // berechnet den Wert des Neurons
        void calc_value();

        // alle Verbindungen von diesem Neuron zu vorherigen Neuronen
        std::vector<Connection> connections;

        // Aktivierungsfunktion: Leaky ReLU
        float activation_function(float input);
};
