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
	gcc objects/*.o -o $name -std=c23
}

clean() {
	rm -r objects
}

build() {
	compile
	link
}

install() {
	cp $name /usr/bin/
}

main() {
	local time=$(date +%s)

	local arg
	for arg in "$@"; do
		$arg
	done

	local dtime=$(($(date +%s)-$time))
	echo -e "Done in $dtime seconds. In C++ it would've been $((($dtime + 1) * 25)) at least... :)"
}

main "$@"
