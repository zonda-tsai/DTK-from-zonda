#!/bin/bash

sudo apt update
sudo apt install -y gcc g++ make valgrind

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

gcc -O3 "$DIR/settings.c" -o "$DIR/settings" >/dev/null 2>&1
gcc -O3 "$DIR/compile.c" -o "$DIR/compile" >/dev/null 2>&1
gcc -O3 "$DIR/run.c" -o "$DIR/run" >/dev/null 2>&1
gcc -O3 "$DIR/clean.c" -o "$DIR/clean" >/dev/null 2>&1
"$DIR/settings" -initial
mkdir -p "$HOME/bin"
mv "$DIR"/{settings,compile,run,clean} "$HOME/bin"
mkdir -p "$HOME/.zonda.ide"
mv "$DIR"/{settings.c,compile.c,run.c,clean.c} "$HOME/.zonda.ide"

if ! grep -q 'export PATH="$HOME/bin:$PATH"' "$HOME/.bashrc"; then
    echo 'export PATH="$HOME/bin:$PATH"' >> "$HOME/.bashrc"
fi
source $HOME/.bashrc

printf "\e[33;1m[FINISHED]\e[0m\n"
