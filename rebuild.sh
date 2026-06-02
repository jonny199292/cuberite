#!/bin/bash
set -e

# 1. Detectare automată a numărului maxim de thread-uri disponibile pe CPU
THREADS=$(nproc)
echo " [OFFLINE BUILD] Detectat hardware nativ. Se folosesc toate cele $THREADS thread-uri disponibile pentru compilare paralelă..."

# 2. Curățare brutală (Inclusiv cache-ul ascuns și configurările FASTER)
echo " Curățare directoare și cache CMake..."
cd "$(dirname "$0")" # Ne asigurăm că scriptul rulează în folderul lui
rm -rf build
rm -rf build-root
rm -f CMakeCache.txt
rm -rf CMakeFiles

# 3. Re-creare folder de build curat
mkdir build
cd build

# 4. Configurare CMake (Aici se vor aplica flag-urile de C++23 și -march=native din CMakeLists.txt)
echo "🛠️ Configurare proiect cu CMake în standard modern..."
cmake -DCMAKE_BUILD_TYPE=Release ..

# 5. Compilare în forță folosind toată puterea procesorului
echo "Se compilează Cuberite Core pe $THREADS fire de execuție..."
make -j"$THREADS" Cuberite #VERBOSE=1

echo " Rebuild complet! Binarul C++23 cu suport FASTER se află la: build/Server/Cuberite"

