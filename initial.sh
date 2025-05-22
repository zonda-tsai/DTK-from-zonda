#!/bin/bash

sudo apt update
sudo apt install -y gcc g++ make valgrind

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

gcc -O3 "$DIR/settings.c" -o "$DIR/settings" >/dev/null 2>&1
gcc -O3 "$DIR/compile.c" -o "$DIR/compile" >/dev/null 2>&1
gcc -O3 "$DIR/run.c" -o "$DIR/run" >/dev/null 2>&1
gcc -O3 "$DIR/char_ins.c" -o "$DIR/char_ins" >/dev/null 2>&1
"$DIR/settings" -initial
mkdir -p "$HOME/bin"
mv "$DIR/settings" "$DIR/compile" "$DIR/run" "$DIR/char_ins" "$HOME/bin"
mkdir -p "$HOME/.zonda.ide"
mv "$DIR/settings.c" "$DIR/compile.c" "$DIR/run.c" "$DIR/char_ins.c" "$DIR/README.md" "$HOME/.zonda.ide"

if ! grep -q 'export PATH="$HOME/bin:$PATH"' "$HOME/.bashrc"; then
    echo 'export PATH="$HOME/bin:$PATH"' >> "$HOME/.bashrc"
fi
source $HOME/.bashrc
export PATH="$HOME/bin:$PATH"

rm -rf "$DIR"
printf "\e[33;1m[FINISHED]\e[0m\n"
