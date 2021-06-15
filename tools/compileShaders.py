#!/usr/bin/python
import sys
import os

GLSLC = ''

if sys.platform == "linux":
    GLSLC = '../bin/glslangValidator'
elif sys.platform == "win32":
    GLSLC = '../bin/glslangValidator.exe'

SHADERS_DIR = '../shaders/'
BUILD_DIR = '../bin/shaders/'

if __name__ == "__main__":
    print("Compile Shaders!")

    if not os.path.exists(BUILD_DIR):
        os.mkdir(BUILD_DIR)

    for shader in os.listdir(SHADERS_DIR):
        cmd = GLSLC + ' -V -o ' + (BUILD_DIR + shader + '.spv') + ' ' + (SHADERS_DIR + shader)
        print(cmd)
        os.system(cmd)
