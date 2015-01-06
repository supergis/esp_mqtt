#!/bin/bash

echo "================================="
echo "commit all source to oschina..."
echo "================================="

git add ./ -A
git commit -a -m "update ..."
git push
