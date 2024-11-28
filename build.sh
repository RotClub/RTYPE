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
    DCMAKE_BUILD_TYPE=Release
    BUILD_FOLDER=cmake-build-release
elif [ "$1" = "profile" ]; then
    echo "Building as profile"
    DCMAKE_BUILD_TYPE=RelWithDebInfo
    BUILD_FOLDER=cmake-build-relwithdebinfo
else
    echo "Building as debug"
    DCMAKE_BUILD_TYPE=Debug
    BUILD_FOLDER=cmake-build-debug
fi

# Configure
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="$DCMAKE_BUILD_TYPE" -B "$BUILD_FOLDER" .

# Build
if [ -n "$TARGET" ]; then
  cmake --build "$BUILD_FOLDER" --target r-type_"$TARGET" -j $(nproc)
else
  cmake --build "$BUILD_FOLDER" -j $(nproc)
fi
