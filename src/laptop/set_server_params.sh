#!/bin/bash

if [ "$1" = "-D" ]
    then 
        echo "Setting default params"
        shift

        camera_modes="\"imx219.json\""

        sensor_mode=0
        display_width=640
        display_height=360
        flip_method0=0
        flip_method1=0
        
        left0=0
        right0=$display_width
        top0=0
        bottom0=$display_height

        left1=0
        right1=$display_width
        top1=0
        bottom1=$display_height

        concat_type=0

        mtu=1400
        host="\"192.168.0.105\""
        port=6666

    else
        echo "Use previous params"

        camera_modes=$(cat 'params.json' | jq '.capture_params.camera_modes')

        sensor_mode=$(cat 'params.json' | jq '.capture_params.sensor_mode')
        display_width=$(cat 'params.json' | jq '.capture_params.display_width')
        display_height=$(cat 'params.json' | jq '.capture_params.display_height')
        flip_method0=$(cat 'params.json' | jq '.capture_params.flip_method0')
        flip_method1=$(cat 'params.json' | jq '.capture_params.flip_method1')

        left0=$(cat 'params.json' | jq '.capture_params.left0')
        right0=$(cat 'params.json' | jq '.capture_params.right0')
        top0=$(cat 'params.json' | jq '.capture_params.top0')
        bottom0=$(cat 'params.json' | jq '.capture_params.bottom0')

        left1=$(cat 'params.json' | jq '.capture_params.left1')
        right1=$(cat 'params.json' | jq '.capture_params.right1')
        top1=$(cat 'params.json' | jq '.capture_params.top1')
        bottom1=$(cat 'params.json' | jq '.capture_params.bottom1')

        concat_type=$(cat 'params.json' | jq '.capture_params.concat_type')

        mtu=$(cat 'params.json' | jq '.stream_params.mtu')
        host=$(cat 'params.json' | jq '.stream_params.host')
        port=$(cat 'params.json' | jq '.stream_params.port')

        # [ -z "$camera_modes" ] && camera_modes="\"imx219.json\"" || 1

        # [ $left0=~  ] && left0=0 || 1
        # [ -z "$right0" ] && right0=$display_width || 1
        # [ -z "$top0" ] && top0=0 || 1
        # [ -z "$bottom0" ] && bottom0=$display_height || 1

        # [ -z "$left1" ] && left1=0 || 1
        # [ -z "$right1" ] && right1=$display_width || 1
        # [ -z "$top1" ] && top1=0 || 1
        # [ -z "$bottom1" ] && bottom1=$display_height || 1

        # [ -z "$host" ] && host="\"192.168.0.105\"" || 1
fi

while [ "$#" -gt 0 ]
    do
        case $1 in 
            --camera_modes) camera_modes="\"$2\""; shift;;

            --sensor_mode) sensor_mode="$2"; shift;;
            --display_width) display_width="$2"; shift;;
            --display_height) display_height="$2"; shift;;
            --flip_method0) flip_method0="$2"; shift;;
            --flip_method1) flip_method1="$2"; shift;;

            --left0) left0="$2"; shift;;
            --right0) right0="$2"; shift;;
            --top0) top0="$2"; shift;;
            --bottom0) bottom0="$2"; shift;;

            --left1) left1="$2"; shift;;
            --right1) right1="$2"; shift;;
            --top1) top1="$2"; shift;;
            --bottom1) bottom1="$2"; shift;;

            --concat_type) concat_type="$2"; shift;;

            --mtu) mtu="$2"; shift;;
            --host) host="\"$2\""; shift;;
            --port) port="$2"; shift;;
        esac
        shift
done

# {
#     jq -cn --arg camera_modes $camera_modes \
#     --arg sensor_mode $sensor_mode \
#     '{capture_params: $ARGS.named}'
#     jq -cn --arg stream_params $camera_modes '{stream_params: $ARGS.named}'
# } | jq -s add > params.json

# jq -n --arg capture_params $camera_modes > params.json

echo "{
    \"capture_params\": {
        \"camera_modes\": $camera_modes,

        \"sensor_mode\": $sensor_mode,
        \"display_width\": $display_width,
        \"display_height\": $display_height,
        \"flip_method0\": $flip_method0,
        \"flip_method1\": $flip_method1,

        \"left0\": $left0,
        \"right0\": $right0,
        \"top0\": $top0,
        \"bottom0\": $bottom0,

        \"left1\": $left1,
        \"right1\": $right1,
        \"top1\": $top1,
        \"bottom1\": $bottom1,

        \"concat_type\": $concat_type
    },
    \"stream_params\": {
        \"mtu\": $mtu,
        \"host\": $host,
        \"port\": $port
    }
}" > server_params.json

cat 'server_params.json'
