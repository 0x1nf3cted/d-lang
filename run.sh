#!/bin/sh

cd build && cmake .. && make && ./dzair ../vars.dz && sleep 1 && cd ..