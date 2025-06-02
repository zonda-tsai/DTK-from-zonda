#!/bin/bash

printf "\x1b[33;1mDownload gcc, g++, make, valgrind and glow\x1b[0m\n"
sudo apt update && sudo apt upgrade
sudo apt install -y gcc g++ make valgrind
sudo snap install glow

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

gcc -O3 "$DIR/settings.c" -o "$DIR/settings" >/dev/null 2>&1
gcc -O3 "$DIR/compile.c" -o "$DIR/compile" >/dev/null 2>&1
gcc -O3 "$DIR/run.c" -o "$DIR/run" >/dev/null 2>&1
gcc -O3 "$DIR/clean.c" -o "$DIR/clean" >/dev/null 2>&1
gcc -O3 "$DIR/char_ins.c" -o "$DIR/char_ins" >/dev/null 2>&1
gcc -O3 "$DIR/tfmanager.c" -o "$DIR/tfmanager" >/dev/null 2>&1
"$DIR/settings" -initial
mkdir -p "$HOME/bin"
mv "$DIR/settings" "$DIR/compile" "$DIR/run" "$DIR/clean" "$DIR/char_ins" "$DIR/tfmanager" "$HOME/bin"
mkdir -p "$HOME/.zonda.ide"
mv "$DIR/README.md" "$HOME/.zonda.ide"
mkdir -p "$HOME/.zonda.ide/sources"
mv "$DIR/settings.c" "$DIR/compile.c" "$DIR/run.c" "$DIR/clean.c" "$DIR/char_ins.c" "$DIR/tfmanager.c" "$HOME/.zonda.ide/sources"

if ! grep -q 'export PATH="$HOME/bin:$PATH"' "$HOME/.bashrc"; then
    echo 'export PATH="$HOME/bin:$PATH"' >> "$HOME/.bashrc"
fi
source $HOME/.bashrc
export PATH="$HOME/bin:$PATH"

rm -rf "$DIR"
printf "\n\e[33;1m[FINISHED]\e[0m\n"
printf "If you can't successfully use compile or othere executable files, please type 'export PATH=\"$HOME/bin:$PATH\"' in your terminal\n"
