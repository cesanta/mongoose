#!/bin/bash

cd /data/downloads/health
find . -mtime +28 -not -name 'index*.json' -delete
find . -name "*.json" -not -name 'index*.json' | jq -R . | jq -s . > index.json
