#!/bin/sh

sudo apt-get -y install mosquitto
sudo cp test/mosquitto.conf /etc/mosquitto/conf.d/
sudo cp test/certs/ca.crt /etc/mosquitto/certs/
sudo cp test/certs/server.crt /etc/mosquitto/certs/
sudo cp test/certs/server.key /etc/mosquitto/certs/
sudo chmod a+w /var/log/mosquitto/*
sudo mosquitto -c /etc/mosquitto/mosquitto.conf &
