#!/bin/bash

if [ ! -d "project" ]; then
    mkdir project
fi

cd project
cmake ..
