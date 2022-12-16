#!/bin/bash
mkdir out 2>/dev/null
magick maze.png -sample 100x100 out/maze-resized.png
magick out/maze-resized.png -color-threshold 'sRGB(0,0,0)-sRGB(160,160,160)' -negate out/map.png
magick out/maze-resized.png -color-threshold 'sRGB(0,0,200)-sRGB(160,160,255)' -negate out/start.png
magick out/maze-resized.png -color-threshold 'sRGB(200,0,0)-sRGB(255,160,160)' -negate out/end.png
rm out/maze-resized.png