#include "ai_hub.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void ai_hub::_bind_methods() {

    ClassDB::bind_method(D_METHOD("get_generation"), &ai_hub::get_generation);
    ClassDB::bind_method(D_METHOD("set_generation", "generation"), &ai_hub::set_generation);

    ClassDB::bind_method(D_METHOD("get_best_score"), &ai_hub::get_best_score);
    ClassDB::bind_method(D_METHOD("set_best_score", "bestScore"), &ai_hub::set_best_score);

    ClassDB::bind_method(D_METHOD("get_train_ai"), &ai_hub::get_train_ai);
    ClassDB::bind_method(D_METHOD("set_train_ai", "trainAi"), &ai_hub::set_train_ai);

    ClassDB::bind_method(D_METHOD("get_num_players"), &ai_hub::get_num_players);
    ClassDB::bind_method(D_METHOD("set_num_players", "numPlayers"), &ai_hub::set_num_players);

    ClassDB::bind_method(D_METHOD("get_mut_chance"), &ai_hub::get_mut_chance);
    ClassDB::bind_method(D_METHOD("set_mut_chance", "mutChance"), &ai_hub::set_mut_chance);

    ClassDB::bind_method(D_METHOD("get_weight_mut_strength"), &ai_hub::get_weight_mut_strength);
    ClassDB::bind_method(D_METHOD("set_weight_mut_strength", "weightMutStrength"), &ai_hub::set_weight_mut_strength);

    ClassDB::bind_method(D_METHOD("get_bias_mut_strength"), &ai_hub::get_bias_mut_strength);
    ClassDB::bind_method(D_METHOD("set_bias_mut_strength", "biasMutStrength"), &ai_hub::set_bias_mut_strength);

    ClassDB::bind_method(D_METHOD("get_ai_player_group_name"), &ai_hub::get_ai_player_group_name);
    ClassDB::bind_method(D_METHOD("set_ai_player_group_name", "groupName"), &ai_hub::set_ai_player_group_name);

    ClassDB::bind_method(D_METHOD("get_nns"), &ai_hub::get_nns);
    ClassDB::bind_method(D_METHOD("set_nns"), &ai_hub::set_nns);

    ClassDB::bind_method(D_METHOD("sort_nns_on_score"), &ai_hub::sort_nns_on_score);

    
    ADD_PROPERTY(PropertyInfo(Variant::INT, "generation"), "set_generation", "get_generation");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "best_score"), "set_best_score", "get_best_score");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "train_ai"), "set_train_ai", "get_train_ai");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "num_players"), "set_num_players", "get_num_players");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mut_chance"), "set_mut_chance", "get_mut_chance");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "weight_mut_strength"), "set_weight_mut_strength", "get_weight_mut_strength");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "bias_mut_strength"), "set_bias_mut_strength", "get_bias_mut_strength");
    

    // ClassDB::bind_method(D_METHOD("set_yPos", "yPos"), &ai_hub::set_yPos);
    // ClassDB::bind_method(D_METHOD("set_layers", "layer_layout"), &ai_hub::set_layers);
    // ClassDB::bind_method(D_METHOD("get_layers"), &ai_hub::get_layers);
    // ClassDB::bind_method(D_METHOD("set_weights_and_biases", "weights_and_biases"), &ai_hub::set_weights_and_biases);
    // ClassDB::bind_method(D_METHOD("get_weights_and_biases"), &ai_hub::get_weights_and_biases);
    // ClassDB::bind_method(D_METHOD("solve", "Input"), &ai_hub::solve);
    // ClassDB::bind_method(D_METHOD("fill_connections"), &ai_hub::fill_connections);
    // //ClassDB::add_property("yPos", PropertyInfo(Variant::FLOAT, "yPos"), "set_yPos", "get_yPos");
    // ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "yPos"), "set_yPos", "get_yPos");
    //ADD_SIGNAL(MethodInfo("lol_signal", PropertyInfo(Variant::INT, "number")));

}


ai_hub::ai_hub() {
    if (Engine::get_singleton()->is_editor_hint()) {
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    } 
}


void ai_hub::sort_nns_on_score() {
    nns.sort();
}