#!/usr/bin/env bash
set -e

echo ""
echo "Building Rakkit..."
clang++ -std=c++17 src/*.cpp -Iinclude -o rak
echo ""
echo "---------------------------"
echo ""
echo "Installing Rakkit to /usr/local/bin requires password"
sudo mv rak /usr/local/bin/rak
echo ""
echo "---------------------------"
echo ""
echo "Installed Successfully!"
echo ""
echo "---------------------------"
echo ""
echo "to run your file use 'rak race <directory>'"
echo "to view contents of a file use 'rak peep <directory>'"
echo ""
echo "---------------------------"
echo ""
echo "Try; rak race test/hello.rk"
echo "or; rak peep test/hello.rk"
echo ""
echo "~~~~~~~"
