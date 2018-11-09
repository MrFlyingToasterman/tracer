#! /bin/bash
echo "[INFO] Compiling tracer"
echo "[INFO] Please wait ..."
g++ tracer.cxx -o bin/tracer-bin
echo "[INFO] Ecec ./bin/tracer-bin"
sudo bin/tracer-bin
