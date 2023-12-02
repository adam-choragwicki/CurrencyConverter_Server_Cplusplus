#!/bin/bash

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./bin
export LD_LIBRARY_PATH

./bin/CurrencyConverter_Server
