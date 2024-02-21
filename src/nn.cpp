#include "nn.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <algorithm>
#include <cstring>

using namespace godot;

void nn::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_yPos"), &nn::get_yPos);
    ClassDB::bind_method(D_METHOD("set_yPos", "yPos"), &nn::set_yPos);

    ClassDB::bind_method(D_METHOD("set_layers", "layer_layout"), &nn::set_layers);
    ClassDB::bind_method(D_METHOD("get_layers"), &nn::get_layers);

    ClassDB::bind_method(D_METHOD("set_weights_and_biases", "weights_and_biases"), &nn::set_weights_and_biases);
    ClassDB::bind_method(D_METHOD("get_weights_and_biases"), &nn::get_weights_and_biases);

    ClassDB::bind_method(D_METHOD("solve", "Input"), &nn::solve);
    ClassDB::bind_method(D_METHOD("fill_connections"), &nn::fill_connections);

    //ClassDB::add_property("yPos", PropertyInfo(Variant::FLOAT, "yPos"), "set_yPos", "get_yPos");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "yPos"), "set_yPos", "get_yPos");

    //ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "layer_sizes"), "set_layers", "get_layers");


    //ADD_SIGNAL(MethodInfo("lol_signal", PropertyInfo(Variant::INT, "number")));

}

nn::nn() {
    if (Engine::get_singleton()->is_editor_hint()) {
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    } 
}


void nn::set_yPos(const double yPos) {
    this->yPos = yPos;
}
double nn::get_yPos() const {
    return yPos;
}

void nn::set_layers(godot::TypedArray<int> layer_layout) {
    std::vector<int> sizes;

    // std::for_each(&layer_layout.front(), (&layer_layout.back()), [&sizes](int size){
        
    // });

    for (int i = 0; i < layer_layout.size(); i++) {
        sizes.push_back(layer_layout[i]);
    }

    layers = create_layers(sizes);
}


godot::TypedArray<int> nn::get_layers() const {

    godot::TypedArray<int> layer_layout;

    for (const Layer& layer : layers) {
        layer_layout.push_back(layer.neurons.size());
    }

    UtilityFunctions::print(layer_layout);

    return layer_layout;
}


std::vector<Layer> create_layers(std::vector<int> layer_layout) {
    std::vector<Layer> layers;
    for (int id = 0; id < layer_layout.size(); id++) {
        layers.push_back(Layer(layer_layout[id], id));
    }
    return layers;
}

void nn::fill_connections() {
    Layer* past_layer{nullptr};

    for (Layer& layer : layers) {
        if (past_layer)
        {
            for(Neuron& neuron : layer.neurons)
            {
                neuron.connections.clear();
                for(Neuron& past_neuron : past_layer->neurons) {
                    neuron.connections.push_back(Connection{0.0f, 0.0f, past_neuron});
                }
            }
        }
        past_layer = &layer;
    }
}

std::vector<float> nn::float_serialize(const std::vector<Layer>& layers) const {
    std::vector<float> weights_and_biases;

    for (const Layer& layer : layers) {
        for (const Neuron& neuron : layer.neurons) {
            for (const Connection& connection : neuron.connections) {
                weights_and_biases.push_back(connection.weight);
            }
        }
    }

    return weights_and_biases;
}

std::vector<std::byte> nn::serialize() const {
    std::vector<std::byte> bytes;

    std::vector<float> values = nn::float_serialize(layers);

    bytes.resize(values.size() * sizeof(float));

    std::memcpy(bytes.data(), values.data(), bytes.size());

    return bytes;
}


void nn::float_deserialize(std::vector<float> &weights_and_biases) {
    int index = 0;
    for (Layer& layer : layers) {
        for (Neuron& neuron : layer.neurons) {
            for (Connection& connection : neuron.connections) {
                connection.weight = weights_and_biases.at(index);
                connection.bias = weights_and_biases.at(index + 1);

                index += 2;
            }
        }
    }
}

void nn::deserialize(const std::vector<std::byte> &binary) {
    
}



void nn::set_weights_and_biases(godot::TypedArray<float> weights_and_biases) {
    nn::weights_and_biases.clear();
    for (int i = 0; i < weights_and_biases.size(); i++) {
        nn::weights_and_biases.push_back(weights_and_biases[i]);
    }
    nn::float_deserialize(nn::weights_and_biases);
}

godot::TypedArray<float> nn::get_weights_and_biases() const {
    godot::TypedArray<float> weights_and_biases;

    for (const float& weight_or_bias : nn::weights_and_biases) {
        weights_and_biases.push_back(weight_or_bias);
    }

    return weights_and_biases;
}


godot::TypedArray<float> nn::solve(godot::TypedArray<float> Inputs) {
    {
        int i = 0;
        for (Neuron &neuron : layers.at(0).neurons) {
            neuron.value = Inputs[i];
            i++;
        }
    }

    for (int i = 1; i < layers.size(); i++) {
        layers.at(i).calc_values();
    }

    Layer output_layer = layers.at(layers.size()-1);


    godot::TypedArray<float> outputs;
    for (Neuron &neuron : output_layer.neurons) {
        outputs.push_back(neuron.value);
    }

    return outputs;
}

void nn::mutate(float strength) {
    
}