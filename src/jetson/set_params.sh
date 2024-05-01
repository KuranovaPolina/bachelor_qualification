capture_width=640
capture_height=360
display_width=640
display_height=360
framerate=120
format=2

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
