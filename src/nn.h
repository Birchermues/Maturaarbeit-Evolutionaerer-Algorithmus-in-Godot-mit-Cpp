#pragma once

//#include "../godot-cpp/gen/include/godot_cpp/classes/node.hpp"

#include <godot_cpp/classes/node.hpp>
#include "neuron.h"
#include "layer.h"

#include <vector>


namespace godot {
    class nn: public Node {
        GDCLASS(nn, Node);
                            
        private:
            double yPos;
        
        protected:
            static void _bind_methods();

        public:
            nn();
            void set_yPos(const double yPos);
            double get_yPos() const;

            void _process(double delta);

            void set_layers(TypedArray<int> layer_layout);
            TypedArray<int> get_layers() const;

            std::vector<Layer> layers;

            std::vector<std::byte> serialize() const;

            void deserialize(const std::vector<std::byte>& binary);
            
            
    };
}

std::vector<Layer> create_layers(std::vector<int> layer_layout);
