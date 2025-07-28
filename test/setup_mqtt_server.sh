#!/bin/sh

sudo apt-get -y install mosquitto
# "for some reason" they think starting the server is cool
sudo cp test/mosquitto.conf /etc/mosquitto/conf.d/
sudo cp test/certs/ca.crt /etc/mosquitto/certs/
sudo cp test/certs/server.crt /etc/mosquitto/certs/
sudo cp test/certs/server.key /etc/mosquitto/certs/
sudo ss -tpln | grep 883
# so we need to restart it with the actual config
sudo systemctl restart mosquitto
sudo ss -tpln | grep 883
sudo cat /var/log/mosquitto/*
