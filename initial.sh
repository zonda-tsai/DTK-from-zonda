#!/bin/bash

printf "\x1b[33;1mDownload gcc, g++, make and valgrind\x1b[0m\n"
sudo apt update && sudo apt upgrade
sudo apt install -y gcc g++ make valgrind

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

gcc -O3 "$DIR/settings.c" -o "$DIR/settings" >/dev/null 2>&1
gcc -O3 "$DIR/compile.c" -o "$DIR/compile" >/dev/null 2>&1
gcc -O3 "$DIR/run.c" -o "$DIR/run" >/dev/null 2>&1
gcc -O3 "$DIR/clean.c" -o "$DIR/clean" >/dev/null 2>&1
gcc -O3 "$DIR/char_ins.c" -o "$DIR/char_ins" >/dev/null 2>&1
gcc -O3 "$DIR/tfmanager.c" -o "$DIR/tfmanager" >/dev/null 2>&1
gcc -O3 "$DIR/+.c" -o "$DIR/+" >/dev/null 2>&1
gcc -O3 "$DIR/tutorial.c" -o "$DIR/tutorial" >/dev/null 2>&1
"$DIR/settings" -initial
mkdir -p "$HOME/bin"
mv "$DIR/settings" "$DIR/compile" "$DIR/run" "$DIR/clean" "$DIR/char_ins" "$DIR/tfmanager" "$DIR/+" "$HOME/bin"
mkdir -p "$HOME/.zonda.due"
mv "$DIR/tutorial" "$HOME/.zonda.due"
mkdir -p "$HOME/.zonda.due/sources"
mv "$DIR/settings.c" "$DIR/tutorial.c" "$DIR/compile.c" "$DIR/run.c" "$DIR/clean.c" "$DIR/char_ins.c" "$DIR/tfmanager.c" "$DIR/+.c" "$HOME/.zonda.due/sources"

if ! grep -q 'export PATH="$HOME/bin:$PATH"' "$HOME/.bashrc"; then
    echo 'export PATH="$HOME/bin:$PATH"' >> "$HOME/.bashrc"
fi
source $HOME/.bashrc
export PATH="$HOME/bin:$PATH"

rm -rf "$DIR"
printf "\n\e[33;1m[FINISHED]\e[0m\n"
printf "If you can't successfully use compile or othere executable files, please type 'export PATH=\"$HOME/bin:$PATH\"' in your terminal\n"
printf "If you are new to this, please type 'settings --help' for tutorial\n"
