# !/bin/bash
FILE=harbour-swisshockey
declare -a RESOLUTIONS=(86 108 128 256)
for RES in "${RESOLUTIONS[@]}"; do
    inkscape -f "scalable/${FILE}.svg" -e "${RES}x${RES}/${FILE}.png" -w ${RES} -h ${RES}
done

