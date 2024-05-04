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
        flip_method=0

        mtu=1400
        host="\"192.168.0.101\""
        port=6666
        concat_type=0

    else
        echo "Use previous params"

        capture_width=$(cat 'params.json' | jq '.stream_params.capture_width')
        capture_height=$(cat 'params.json' | jq '.stream_params.capture_height')
        display_width=$(cat 'params.json' | jq '.stream_params.display_width')
        display_height=$(cat 'params.json' | jq '.stream_params.display_height')
        framerate=$(cat 'params.json' | jq '.stream_params.framerate')
        flip_method=$(cat 'params.json' | jq '.stream_params.flip_method')

        mtu=$(cat 'params.json' | jq '.stream_params.mtu')
        host=$(cat 'params.json' | jq '.stream_params.host')
        port=$(cat 'params.json' | jq '.stream_params.port')
        concat_type=$(cat 'params.json' | jq '.stream_params.concat_type')
fi

while [ "$#" -gt 0 ]
    do
        case $1 in 
            --capture_width) capture_width="$2"; shift;;
            --capture_height) capture_height="$2"; shift;;

            --display_width) display_width="$2"; shift;;
            --display_height) display_height="$2"; shift;;

            --framerate) framerate="$2"; shift;;

            --flip_method) flip_method="$2"; shift;;

            --mtu) mtu="$2"; shift;;
            --host) host="\"$2\""; shift;;

            --port) port="$2"; shift;;
            --concat_type) concat_type="$2"; shift;;
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
        \"flip_method\": $flip_method,

        \"mtu\": $mtu,
        \"host\": $host,
        \"port\": $port,
        \"concat_type\": $concat_type        
    }
}" > params.json

cat 'params.json'
