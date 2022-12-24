# Build App for the MacOS
# For MacOS, cmake will pop up a window:
# * Path and Build Path: the path to src code, e.g. ../picap/libdatachannel
# * Current generator: Unix Makefiles
# * Click "configure", then "generate"
# * Close the window, then it will start to compile
# The product should be picap

import sys
import os
from os import path
import shutil


cmake_download = """
git clone https://github.com/paullouisageneau/libdatachannel.git
cp -r ../picap ./libdatachannel/examples/
rm -rf ./libdatachannel/examples/picap/libdatachannel
"""
cmake_build = """
cd libdatachannel
git submodule update --init --recursive --depth 1
/Applications/CMake.app/Contents/bin/cmake-gui -B build -DUSE_GNUTLS=0 -DUSE_NICE=0 -DCMAKE_BUILD_TYPE=Release
cd build
make -j2
"""

CMAKE_PATH = '/Applications/CMake.app/Contents/bin/cmake-gui'

if not path.exists(CMAKE_PATH):
    print(CMAKE_PATH, 'is not installed')
    exit()

if not path.exists('./libdatachannel'):
    os.system(cmake_download)
    try:
        with open('./libdatachannel/CMakeLists.txt', 'a') as cmake:
            cmake.write('add_subdirectory(examples/picap)\n')
    except Exception as err:
        print(err)
        os.system('rm -rf ./libdatachannel')
        exit()
    os.system(cmake_build)

os.system('cd ./libdatachannel/examples/picap ; make clean; make')
os.system('rm -rf picap')
shutil.copyfile('./libdatachannel/examples/picap/picap', './picap')
os.system('chmod +x picap')



