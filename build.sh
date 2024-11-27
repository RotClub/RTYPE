#!/bin/bash

# Get the script arguments
if [ -z "$1" ]; then
  echo "Usage: $0 <build_type> (target)"
  echo "  build_type: 'debug', 'release' or 'profile'"
  echo "  target: 'engine', 'server', 'client' or empty"
  exit 1
fi

TARGET=""
if [ -z "$2" ]; then
  TARGET=""
elif [ "$2" = "engine" ] || [ "$2" = "server" ] || [ "$2" = "client" ];then
  TARGET="$2"
else
  echo "If you choose to specify the target, it needs to be either 'engine', 'server' or 'client'"
fi

if [ "$1" = "release" ] || [ -z "$DISPLAY" ]; then
    echo "Building as release"
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -B cmake_build .
elif [ "$1" = "profile" ]; then
    echo "Building as profile"
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -B cmake_build .
else
    echo "Building as debug"
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -B cmake_build ..
fi

# Build
if [ -n "$TARGET" ]; then
  cmake --build .build --target r-type_"$TARGET" -j $(nproc)
else
  cmake --build .build -j $(nproc)
fi
