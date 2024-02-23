#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")


env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demoproject/bin/nn.{}.{}.framework/nn.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "demoproject/bin/nn{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

if env["platform"] == "macos":
    library2 = env.SharedLibrary(
        "demoproject/bin/ai_hub.{}.{}.framework/ai_hub.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library2 = env.SharedLibrary(
        "demoproject/bin/ai_hub{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
Default(library2)
