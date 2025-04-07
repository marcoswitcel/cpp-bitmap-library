#!/usr/bin/env sh


make main

cd ./image

../target/bmp_tool --file-in flower_bi_rgb.bmp --file-out flower_bi_rgb.out.bmp --verbose --export-sample --generate-image
../target/bmp_tool --file-in flower_bi_rgb.bmp --file-out flower_bi_rgb.2x.out.bmp --verbose --resize --width 3840 --height 2160
../target/bmp_tool --file-in flower_bi_rgb.bmp --file-out flower_bi_rgb.green.out.bmp --verbose --filter green_channel
../target/bmp_tool --file-in flower_bi_rgb.bmp --file-out flower_bi_rgb.gray.out.bmp --verbose --filter gray

../target/bmp_tool --file-in sample-01-2x2.bmp --file-out sample-01-4x4.bmp --verbose --resize --width 4 --height 4
../target/bmp_tool --file-in sample-01-2x2.bmp --file-out sample-01-200x200.out.bmp --verbose --resize --width 200 --height 200

../target/bmp_tool --file-in image-generated.bmp --file-out image-generated.out.bmp --verbose --filter blue_channel

