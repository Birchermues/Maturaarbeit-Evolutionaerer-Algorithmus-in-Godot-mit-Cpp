#include "ai_hub.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <algorithm>
#include <array>

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

    ClassDB::bind_method(D_METHOD("get_mut_strength"), &ai_hub::get_mut_strength);
    ClassDB::bind_method(D_METHOD("set_mut_strength", "MutStrength"), &ai_hub::set_mut_strength);


    ClassDB::bind_method(D_METHOD("get_ai_player_group_name"), &ai_hub::get_ai_player_group_name);
    ClassDB::bind_method(D_METHOD("set_ai_player_group_name", "groupName"), &ai_hub::set_ai_player_group_name);

    ClassDB::bind_method(D_METHOD("get_nns"), &ai_hub::get_nns);
    ClassDB::bind_method(D_METHOD("set_nns"), &ai_hub::set_nns);

    ClassDB::bind_method(D_METHOD("sort_nns_on_score"), &ai_hub::sort_nns_on_score);
    
    //ClassDB::bind_method(D_METHOD("inherit"), &ai_hub::inherit);
    
    ADD_PROPERTY(PropertyInfo(Variant::INT, "generation"), "set_generation", "get_generation");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "best_score"), "set_best_score", "get_best_score");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "train_ai"), "set_train_ai", "get_train_ai");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "num_players"), "set_num_players", "get_num_players");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mut_chance"), "set_mut_chance", "get_mut_chance");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mut_strength"), "set_mut_strength", "get_mut_strength");
}


ai_hub::ai_hub() {
    if (Engine::get_singleton()->is_editor_hint()) {
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    } 
}


Variant* begin(TypedArray<nn>& array) {
    return &array[0];
}

Variant* end(TypedArray<nn>& array) {
    return std::next(begin(array), array.size());
}


void ai_hub::sort_nns_on_score() {
    Callable custom_func = Callable(this, "custom_sort_func");
    nns.sort_custom(custom_func);
}


// unbenutzte Funktion, welche in godot mit gdscript implementiert ist
void ai_hub::inherit() {
    for (int i = 0; i < nns.size(); i++) {
        Object* obj = nns[i];
        Node *nn_ = Object::cast_to<Node>(obj);
        nn *nn__ = reinterpret_cast<nn*>(nn_);
        if (nn__ == nullptr) {
            UtilityFunctions::print("nullpointer error");
            continue;
        }
        Object* obj2 = nns[0];
        Node *nn2_ = Object::cast_to<Node>(obj2);
        nn *nn2__ = reinterpret_cast<nn*>(nn2_); //FUCK es het nid funktioniert
        if (nn2__ == nullptr) {
            UtilityFunctions::print("nullpointer error 2");
            continue;
        }
        nn__->set_weights_and_biases(nn2__->get_weights_and_biases());
    }
}