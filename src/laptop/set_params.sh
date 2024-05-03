#!/bin/bash


if [ "$1" = "-D" ]
    then 
        echo "Setting default params"
        shift

        port=6666

    else
        echo "Use previous params"

        port=$(cat 'params.json' | jq '.stream_params.port')
fi

while [ "$#" -gt 0 ]
    do
        case $1 in
            --port) port="$2"; shift;;
        esac
        shift
done

echo "{
    \"stream_params\": {
        \"port\": $port      
    }
}" > params.json

cat 'params.json'