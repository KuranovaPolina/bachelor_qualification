#!/bin/bash


if [ "$1" = "-D" ]
    then 
        echo "Setting default params"
        shift

        capture_width=640
        capture_height=360
        display_width=640
        display_height=360
        framerate=120
        format=0

    else
        echo "Use previous params"

        capture_width=$(cat 'params.json' | jq '.stream_params.capture_width')
        capture_height=$(cat 'params.json' | jq '.stream_params.capture_height')
        display_width=$(cat 'params.json' | jq '.stream_params.display_width')
        display_height=$(cat 'params.json' | jq '.stream_params.display_height')
        framerate=$(cat 'params.json' | jq '.stream_params.framerate')
        format=$(cat 'params.json' | jq '.stream_params.format')
fi

while [ "$#" -gt 0 ]
    do
        case $1 in 
            --capture_width) capture_width="$2"; shift;;
            --capture_height) capture_height="$2"; shift;;

            --display_width) display_width="$2"; shift;;
            --display_height) display_height="$2"; shift;;

            --framerate) framerate="$2"; shift;;

            --format) format="$2"; shift;;
        esac
        shift
done

echo "{
    \"stream_params\": {
        \"capture_width\": $capture_width,
        \"capture_height\": $capture_height,
        \"display_width\": $display_width,
        \"display_height\": $display_height,
        \"framerate\": $framerate,
        \"format\": $format
    }
}" > params.json

cat 'params.json'
