#!/bin/bash

display_error() {
    echo "Error: $1" >&2
}

display_success() {
    echo "$1"
}

if [ ! -d "executables/server/executable_linux" ]; then
    mkdir -p executables/server/executable_linux

    if ! wget -q -P executables/server https://github.com/adam-choragwicki/CurrencyConverter_Server_Cplusplus/releases/latest/download/executable_linux.zip; then
        display_error "Failed to download server archive"
        exit 1
    fi
    display_success "Server archive downloaded successfully"

    if ! unzip executables/server/executable_linux.zip -d executables/server > /dev/null; then
        display_error "Failed to unzip server archive"
        exit 1
    fi

    rm executables/server/executable_linux.zip
    display_success "Server archive unzipped successfully"
fi

if [ ! -d "executables/client/release" ]; then
    mkdir -p executables/client/release

    if ! wget -q -P executables/client https://github.com/adam-choragwicki/CurrencyConverter_Client_JavaSwing/releases/latest/download/release.zip; then
        display_error "Failed to download client archive"
        exit 1
    fi
    display_success "Client archive downloaded successfully"

    if ! unzip executables/client/release.zip -d executables/client > /dev/null; then
        display_error "Failed to unzip client archive"
        exit 1
    fi

    rm executables/client/release.zip
    display_success "Client archive unzipped successfully"
fi

pushd .

# Run the server
if ! cd executables/server/executable_linux; then
    display_error "Failed to change directory to server executable directory"
    exit 1
fi

echo "Starting server in another terminal window"
gnome-terminal -- bash -c "./CurrencyConverter_Server; bash"

popd

# Run the client
if ! cd executables/client/release; then
    display_error "Failed to change directory to client release directory"
    exit 1
fi

java -jar CurrencyConverter_Client.jar
