#!/bin/sh

mkdir -p _build

pushd _build

cmake ..
make

popd
