#!/bin/bash


if [ "$1" = "-D" ]
    then 
        echo "Setting default params"
        shift

        port=6666

        concat_type=0

    else
        echo "Use previous params"

        port=$(cat 'params.json' | jq '.stream_params.port')
        concat_type=$(cat 'params.json' | jq '.stream_params.concat_type')
fi

while [ "$#" -gt 0 ]
    do
        case $1 in
            --port) port="$2"; shift;;
            --concat_type) concat_type="$2"; shift;;
        esac
        shift
done

echo "{
    \"stream_params\": {
        \"port\": $port,
        \"concat_type\": $concat_type        
    }
}" > params.json

cat 'params.json'