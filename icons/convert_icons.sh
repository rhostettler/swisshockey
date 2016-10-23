# !/bin/bash

FILE=harbour-swisshockey
RES=86

inkscape -f "scalable/${FILE}.svg" -e "${RES}x${RES}/${FILE}.png" -w ${RES} -h ${RES}
