#!/bin/bash

set -ue 

int_sstrip(){
	SIZE=$(readelf -Wl "$1" | awk '/LOAD/{off=strtonum($2)+strtonum($5)}END{print off}')
	dd if="$1" status=none count=1 bs="$SIZE" of="$2"
	chmod +x "$2"
}

int_sstrip "$1" "${1}-strip"
sstrip "${1}-strip"
#ls -l  "$1" "${1}-strip"

case $(readelf -h "$1" 2>&- |awk '/Class/{print$2}') in
ELF32)
	#echo "is elf32"
	fbdev/whiten.sh "${1}-strip"
	#mv "$1-strip-white" "$1"
esac

IN="${1}-strip-white"
GZ="${1}.gz"
if [ "${RELEASE-1}" -ge 1 ]; then
	#echo RELEASE
	#zopfli --i5000 -c "$IN" > "$GZ"
	#zopfli --i5000 -c --deflate "$IN" > "$1.defl2"

	#rm -f "${IN}.gz"
	cp "$IN" m
	ect -quiet -9 -gzip "m"
	mv "m.gz" "${GZ}"
	#ls -l "$GZ" "${GZ}2"
else
	zopfli -c "$IN" > "$GZ"
	#zopfli -c --deflate "$IN" > "$1.defl2"
fi

OUT="${1}-pack"
printf "\x1f\x8b\x08\x08" > "$OUT"
cat <<'EOF' >> "$OUT"

cp $0 i
zcat<$0>i
./i<>/dev/fb0
EOF
printf "\0" >> "$OUT"
chmod +x "$OUT"
dd status=none if="$GZ" bs=1 skip=12 >> "$OUT"
#rm "$GZ"
#dd if="$GZ" bs=1 skip=10 > "$1.defl"
#zopfli -c --deflate "$1" > "$1.defl2"

#fbdev/defluff < "$OUT" > "$OUT-2" #2>/dev/null
cat < "$OUT" > "$OUT-2" #2>/dev/null

#ls -l "$OUT" "$OUT-2"
fbdev/byteshaver "${OUT}-2" >/dev/null
#ls -l "$OUT" "$OUT-2"
#echo

#mv "${OUT}-2" "$OUT"
#ls -lU  "$1" "${1}-strip" "$OUT"
ls -lU  "$1" "${1}-strip" "$OUT" "${OUT}-2" 
#ls -l "$OUT" "${OUT}-2" "$1.gz" "$1-2.gz" 

##fbdev/sfx.sh "${1}"
#fbdev/byteshaver "${1}-sfx"
