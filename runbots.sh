#!/bin/bash
#
# runbots: run the nuggets game with a bunch of bots
#
# usage:
#   runbots map.txt
#
# David Kotz, May 2019

if [[ $# != 1 || ! -r $1 ]]; then
    echo usage: $0 map.txt
    exit 1
fi
mapfile=$1

function cleanup {
    # kill any wayward child processes
    pkill -P $$
}

# kill child processes if this script is terminated
trap cleanup SIGTERM EXIT

echo ensure the game is built
make all

echo start the server...
./server 2>server.log $mapfile &
sleep 1

echo extract the port number...
serverPort=$(sed -n s/^serverPort=//p server.log)

if [[ "$serverPort" != [0-9]* ]]; then
    echo invalid server port "$serverPort" - exiting...
    exit 2
fi

echo start the player bots...
for let in {A..Z}; do
    ./botclient &>serverunittest/player$let.log localhost $serverPort botbg &
done

echo start the spectator...
./botclient 2>serverunittest/spectator.log localhost $serverPort

echo wait for all to finish...
sleep 1
cleanup
exit 0



