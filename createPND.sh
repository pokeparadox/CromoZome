#!/bin/bash

# Start Over
rm -r ./output/
mkdir ./output
# Prepare Pandora related files
cp -r ./built/font/ ./output/ 
cp -r ./built/images/ ./output/
cp -r ./built/music/ ./output/
cp -r ./built/preview/ ./output/
cp -r ./built/sounds/ ./output/
cp ./built/readme.txt ./output/
cp ./bin/Pandora/CromoZome_R ./output/ 
cp ./built/PXML.xml ./output/
cp ./built/CromoZome.png ./output/
# Create PND files
pnd_make.sh -d ./output -p ./output/cromozome.tmp -c
cat ./output/cromozome.tmp ./output/CromoZome.png > ./output/cromozome.pnd
rm ./output/cromozome.tmp
