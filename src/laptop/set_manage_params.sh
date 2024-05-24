#!/bin/bash


if [ "$1" = "-D" ]
    then 
        echo "Setting default params"
        shift

        port=8185
        host="\"192.168.0.200\""
        params_filename="\"server_params.json\""

    else
        echo "Use previous params"

        port=$(cat 'manage.json' | jq '.stream_params.port')
        host=$(cat 'manage.json' | jq '.stream_params.concat_type')
        params_filename=$(cat 'manage.json' | jq '.stream_params.concat_type')
fi

while [ "$#" -gt 0 ]
    do
        case $1 in
            --port) port="$2"; shift;;
            --host) host="$2"; shift;;
            --params_filename) params_filename="$2"; shift;;
        esac
        shift
done

echo "{
    \"stream_params\": {
        \"port\": $port,
        \"host\": $host,
        \"params_filename\": $params_filename
    }
}" > manage.json

cat 'manage.json'