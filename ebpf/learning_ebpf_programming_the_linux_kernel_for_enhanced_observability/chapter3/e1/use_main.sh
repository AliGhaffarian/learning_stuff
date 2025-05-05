#!/bin/bash
set -e
progname="main"
func1="print_packets"
ifname2attach="neko-tun"
clang -target bpf -I/usr/include/$(uname -m)-linux-gnu -g -c $progname.bpf.c
sudo bpftool prog load $progname.bpf.o /sys/fs/bpf/$progname
progid=$(sudo bpftool prog list | grep "name $func1" | cut -f1 -d:)
echo $progid
sudo bpftool net attach xdp id $progid dev $ifname2attach
