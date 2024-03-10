#include "nn.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <algorithm>
#include <cstring>

using namespace godot;

void nn::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_layers", "layer_layout"), &nn::set_layers);
    ClassDB::bind_method(D_METHOD("get_layers"), &nn::get_layers);

    ClassDB::bind_method(D_METHOD("set_weights_and_biases", "weights_and_biases"), &nn::set_weights_and_biases);
    ClassDB::bind_method(D_METHOD("get_weights_and_biases"), &nn::get_weights_and_biases);

    ClassDB::bind_method(D_METHOD("solve", "Input"), &nn::solve);
    ClassDB::bind_method(D_METHOD("fill_connections"), &nn::fill_connections);

    ClassDB::bind_method(D_METHOD("get_score"), &nn::get_score);
    ClassDB::bind_method(D_METHOD("set_score", "score"), &nn::set_score);

    ClassDB::bind_method(D_METHOD("randomize_weights_and_biases", "use_normal_distribution", "max_weight", "max_bias"), &nn::randomize_weights_and_biases);

    ClassDB::bind_method(D_METHOD("mutate", "mut_chance", "weight_mut_strength", "bias_mut_strength"), &nn::mutate);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score"), "set_score", "get_score");

    //ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "layer_sizes"), "set_layers", "get_layers");


    //ADD_SIGNAL(MethodInfo("lol_signal", PropertyInfo(Variant::INT, "number")));

}

// nn::nn() {
//     if (Engine::get_singleton()->is_editor_hint()) {
//         set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
//     } 
// }


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

    //UtilityFunctions::print(layer_layout);

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


std::partial_ordering nn::operator<=> (const nn &other) const {
    return score <=> other.score;
}

bool nn::operator<(const nn &other) const {
    return score < other.score;
}

void nn::randomize_weights_and_biases(bool use_normal_distribution, float max_weight, float max_bias) {
    int count = 0;

    for (int i = 0; i < layers.size() - 1; i++) {
        count += layers.at(i).neurons.size() * layers.at(i+1).neurons.size();
    }
    weights_and_biases.clear();



    for (int i = 0; i < count; i++) {
        if (use_normal_distribution) {
            weights_and_biases.push_back(godot::UtilityFunctions::randfn(0.0f, max_weight));
            weights_and_biases.push_back(godot::UtilityFunctions::randfn(0.0f, max_bias));
        }
        else {
            weights_and_biases.push_back(godot::UtilityFunctions::randf_range(-max_weight, max_weight));
            weights_and_biases.push_back(godot::UtilityFunctions::randf_range(-max_bias, max_bias));
        }
        

    }
    nn::float_deserialize(nn::weights_and_biases);
    //set_weights_and_biases()
    
}

void nn::mutate(float mut_chance, float weight_mut_strength, float bias_mut_strength) {
    for (int i = 0; i < weights_and_biases.size() / 2; i++) {
        if (godot::UtilityFunctions::randf() < mut_chance) {
            if (godot::UtilityFunctions::randf() < 0.5f) {
                weights_and_biases[i * 2] += godot::UtilityFunctions::randf_range(-weight_mut_strength, weight_mut_strength);
            }
            else {
                weights_and_biases[i * 2 + 1] += godot::UtilityFunctions::randf_range(-bias_mut_strength, bias_mut_strength);
            }
        }
    }
    nn::float_deserialize(nn::weights_and_biases);
}


