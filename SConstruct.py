#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")


env.Append(CPPPATH=["src/"])
# env.Append(SCONS_CXX_STANDARD="c++20")
# env.Append(CXXFLAGS="-std=c++0x")
sources = Glob("src/*.cpp")

# opts = Options()
# opts.Add('RELEASE', 'Set to 1 to build for release', 0)

# env.Append(options=opts, CPPDEFINES={'RELEASE_BUILD' : '${RELEASE}'})


# Require C++20
if env.get("is_msvc", False):
    env.Append(CXXFLAGS=["/std:c++latest"])  # For MSVC
else:
    env.Append(CXXFLAGS=["-std=c++20"])  # For GCC and Clang


if env["platform"] == "macos":
    library = env.SharedLibrary(
        "Game2/bin/nn.{}.{}.framework/nn.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "Game2/bin/nn{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

if env["platform"] == "macos":
    library2 = env.SharedLibrary(
        "Game2/bin/ai_hub.{}.{}.framework/ai_hub.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library2 = env.SharedLibrary(
        "Game2/bin/ai_hub{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
Default(library2)
