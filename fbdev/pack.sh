set -ue 

int_sstrip(){
	SIZE=$(readelf -Wl "$1" | awk '/LOAD/{off=strtonum($2)+strtonum($5)}END{print off}')
	dd if="$1" status=none count=1 bs="$SIZE" of="$2"
	chmod +x "$2"
}

int_sstrip "$1" "${1}-strip"
sstrip "${1}-strip"
ls -l  "$1"
fbdev/whiten.sh "${1}-strip"

#if ! lz4pack "${1}-strip" 2>/dev/null; then
#	ls -l "${1}-strip"
#fi
fbdev/sfx.sh "${1}-strip-white"
