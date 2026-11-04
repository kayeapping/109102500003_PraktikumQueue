#!/bin/bash
set -e
cd "$(dirname "$0")"
mkdir -p bin
clang++ -std=c++17 -Wall -Wextra Soal1/main.cpp Soal1/paket.cpp -o bin/soal1
clang++ -std=c++17 -Wall -Wextra Soal2/main.cpp Soal2/buffer_1_geser.cpp -o bin/soal2_v1
clang++ -std=c++17 -Wall -Wextra Soal2/main.cpp Soal2/buffer_2_pemadatan.cpp -o bin/soal2_v2
clang++ -std=c++17 -Wall -Wextra -DVERSI_CIRCULAR Soal2/main.cpp Soal2/buffer_3_circular.cpp -o bin/soal2_v3
echo "Build selesai. Binary ada di folder bin"
