#!/bin/sh

filename="$1"
if [ -z "$filename" ]; then
	echo "No filename provided!"
else
	cat $filename | tr -d '\040\011\012\015' > "${filename}.min"
	gzip "${filename}.min"
	xxd -i "${filename}.min.gz" "${filename}.c"
	rm "${filename}.min.gz"
fi
