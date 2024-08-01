#!/bin/sh

cd civlike && \
    tl check -q *.tl && \
    tl gen --gen-target 5.1 *.tl && \
    rm -rf ../lib && \
    mkdir -p ../lib && \
    mv *.lua ../lib
