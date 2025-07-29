#!/bin/bash

make

#load time parameters
dev_entry_name="myscull_mem"
mod_name="myscull"
proc_entry_name="proc_myscull"
number_of_devs=3

modprobe ./$mod_name.ko \
	dev_entry_name=$dev_entry_name \
	number_of_devs=$number_of_devs \
	proc_entry_name=$proc_entry_name

MAJOR=$(cat /proc/devices | grep ${dev_entry_name} | cut -f 1  -d ' ')
(
	cd /dev
	for i in $(seq 0 $(($number_of_devs - 1))); do
		mknod ${dev_entry_name}$i c $MAJOR $i
	done
)
