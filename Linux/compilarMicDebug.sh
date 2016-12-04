#!/bin/sh
icpc -O3 -g -mmic -c -fPIC AI.cpp -o AI.o
icpc -O3 -g -mmic -shared -o AI.so AI.o
cp -f AI.so FceuxMic/bin/AI.so
icpc -O3 -g -mmic -c -fPIC Analyzer.cpp -o Analyzer.o
icpc -O3 -g -mmic -shared -o Analyzer.so Analyzer.o
cp -f Analyzer.so FceuxMic/bin/Analyzer.so
