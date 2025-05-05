#!/bin/bash
set -e
progname="main"
ifname2detach="neko-tun"
sudo bpftool net detach xdp dev $ifname2detach
sudo rm /sys/fs/bpf/$progname
