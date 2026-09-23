set -ue 

sstrip(){
	SIZE=$(readelf -Wl "$1" | awk '/LOAD/{off=strtonum($2)+strtonum($5)}END{print off}')
	dd if="$1" status=none count=1 bs="$SIZE" of="$2"
	chmod +x "$2"
}

sstrip "$1" "${1}-strip"
ls -l  "$1"

if ! lz4pack "${1}-strip" 2>/dev/null; then
	ls -l "${1}-strip"
fi
