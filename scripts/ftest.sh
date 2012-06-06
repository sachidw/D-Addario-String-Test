#!/bin/bash
./bin/readFrequency $1
gnuplot scripts/plotFFT.gp
