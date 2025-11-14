#!/usr/bin/env bash

name='serve'

compile() {
	mkdir -p objects

	local file
	for file in src/*.c; do
		echo -e "\e[1;32m * Compiling: $file\e[0m"
		bname=${file##*/}
		bname=${bname%.c}
		gcc -c "$file" -o "objects/$bname.o" -Iinclude
	done
}

link() {
	echo -e "\e[1;32m * Linking\e[0m"
	gcc objects/*.o -o $name
}

clean() {
	rm -r objects
}

build() {
	compile
	link
}

main() {
	for arg in "$@"; do
		$arg
	done
}

main "$@"
