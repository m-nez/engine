#!/bin/bash

DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

cd sdlgl
make
cd ../
make
