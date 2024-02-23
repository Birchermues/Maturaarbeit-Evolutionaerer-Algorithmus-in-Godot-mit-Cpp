#include "register_types.h"
#include "nn.h"
#include "ai_hub.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_nn(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    ClassDB::register_class<nn>();
}

void uninitialize_nn(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

void initialize_ai_hub(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    ClassDB::register_class<ai_hub>();
}

void uninitialize_ai_hub(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}



extern "C"{
    GDExtensionBool GDE_EXPORT nn_init(GDExtensionInterfaceGetProcAddress p_get_proc_adress, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_adress, p_library, r_initialization);

        init_obj.register_initializer(initialize_nn);
        init_obj.register_terminator(uninitialize_nn);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }

    GDExtensionBool GDE_EXPORT ai_hub_init(GDExtensionInterfaceGetProcAddress p_get_proc_adress, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_adress, p_library, r_initialization);

        init_obj.register_initializer(initialize_ai_hub);
        init_obj.register_terminator(uninitialize_ai_hub);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}