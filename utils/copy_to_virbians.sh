#!/bin/bash

for i in {1..4}; do
		if [ -d /tmp/virbian$i/transport ]
		then
				rm -rf /tmp/virbian$i/transport
		fi

		cp -r ~/Documents/Programming/SK/transport /tmp/virbian$i/
done
