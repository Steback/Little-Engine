#!/usr/bin/python
import sys
import os
from pathlib import Path


SHADERS_DIR = 'shaders'
BIN_SHADERS_DIR = 'bin/shaders'

if __name__ == "__main__":
    currentPath = ""

    if len(sys.argv) > 1:
        currentPath = str(str(sys.argv[1]))
    else:
        currentPath = "."

    rootDir = Path(currentPath).absolute()
    if rootDir.name == "tools":
        rootDir = rootDir.parent

    print("Root path:", rootDir)

    shadersDir = rootDir / SHADERS_DIR
    if not shadersDir.exists():
        print("Failed to find shaders dir")
        exit()
    else:
        print("Shaders dir:", shadersDir)

    shadersBinDir = rootDir / BIN_SHADERS_DIR
    if not shadersBinDir.exists():
        os.mkdir(shadersBinDir)

    print("Shaders bin dir:", shadersBinDir)

    glslc = ''
    # TODO: Add Windows .exe file
    if sys.platform == "linux":
        glslc = rootDir / 'bin/glslc'

        if not glslc.exists():
            print("Failed to find glslc file")
            exit()
        else:
            print("glslc file:", glslc, '\n')

    print("COMPILE SHADERS")
    for file in shadersDir.iterdir():
        binFile = shadersBinDir / (file.name + '.spv')
        print(file.name + " >> " + binFile.name)
        os.system(str(glslc.absolute()) + ' ' + str(file.absolute()) + " -o " + str(binFile.absolute()))
