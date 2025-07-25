#!/bin/sh
df
ls -l /mnt
sudo systemctl stop docker
sudo mkdir -p /mnt/docker
sudo cp test/ga_docker_daemon.json /etc/docker/daemon.json
sudo systemctl start docker

