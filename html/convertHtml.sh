#!/bin/sh

filename="$1"
if [ -z "$filename" ]; then
	echo "No filename provided!"
else
	# Can't remove white spaces from HTML
	# http://minifycode.com/html-minifier/
	#cat $filename | tr -d '\040\011\012\015' > "${filename}.min"
	gzip -k "${filename}"
	xxd -i "${filename}.gz" "${filename}.c"
	rm "${filename}.gz"
fi
