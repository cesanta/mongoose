# Remote MQTT Device on an RP2040

Your headless Raspberry Pi Pico-based hardware can also be a remote gadget when you connect it to your computer via USB

For the USB specifics, see this related tutorial at https://mongoose.ws/tutorials/rp2040/pico-rndis-dashboard/

See this tutorial to control your device: https://mongoose.ws/tutorials/mqtt-dashboard/

For this to work, you need your computer to act as a router (gateway) and NAT for your device.

## Linux setup

Enable "masquerading"; the quick and simple options are:

``` bash
sudo iptables --flush
sudo iptables --table nat --flush
sudo iptables --delete-chain
sudo iptables --table nat --delete-chain

# Do masquerading
sudo iptables -t nat -A POSTROUTING -o yourifc -j MASQUERADE
# enable routing
sudo echo 1 > /proc/sys/net/ipv4/ip_forward
```

Where `yourifc` is the interface that is connected to your network

## MacOS setup

