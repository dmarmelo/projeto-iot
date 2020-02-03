#!/bin/sh

# Ro minify the files http://minifycode.com/html-minifier/

filename="$1"
if [ -z "$filename" ]; then
	echo "No filename provided!"
else
	gzip -k "${filename}"
	xxd -i "${filename}.gz" "${filename}.c"
	rm "${filename}.gz"
fi
