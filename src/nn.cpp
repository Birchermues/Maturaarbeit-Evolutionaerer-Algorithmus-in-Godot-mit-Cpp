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

    //ClassDB::add_property("yPos", PropertyInfo(Variant::FLOAT, "yPos"), "set_yPos", "get_yPos");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "yPos"), "set_yPos", "get_yPos");

    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "layer_sizes"), "set_layers", "get_layers");


    //ADD_SIGNAL(MethodInfo("lol_signal", PropertyInfo(Variant::INT, "number")));

}

nn::nn() {
    if (Engine::get_singleton()->is_editor_hint()) {
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    }
    UtilityFunctions::print("update still works");
    
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


void nn::_process(double delta) {
    UtilityFunctions::print("it works every frame");

}



std::vector<Layer> create_layers(std::vector<int> layer_layout) {
    std::vector<Layer> layers;
    for (int id = 0; id < layer_layout.size(); id++) {
        layers.push_back(Layer(layer_layout[id], id));
    }
    return layers;
}

void fill_connections(std::vector<Layer>& layers) {
    Layer* past_layer{nullptr};

    for (Layer& layer : layers) {
        if (past_layer)
        {
            for(Neuron& neuron : layer.neurons)
            {
                for(Neuron& past_neuron : past_layer->neurons) {
                    neuron.connections.push_back(Connection{0.0f, past_neuron});
                }
            }
        }
        past_layer = &layer;
    }
}

std::vector<float> float_serialize(const std::vector<Layer>& layers) {
    std::vector<float> values;

    for (const Layer& layer : layers) {
        for (const Neuron& neuron : layer.neurons) {
            for (const Connection& connection : neuron.connections) {
                values.push_back(connection.weight);
            }
        }
    }

    return values;
}

std::vector<std::byte> nn::serialize() const {
    std::vector<std::byte> bytes;

    auto values = float_serialize(layers);

    bytes.resize(values.size() * sizeof(float));

    std::memcpy(bytes.data(), values.data(), bytes.size());

    return bytes;
}

void godot::nn::deserialize(const std::vector<std::byte> &binary)
{

}
