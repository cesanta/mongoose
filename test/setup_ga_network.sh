#!/bin/bash

# see our network configuration
echo "Network configuration:"
timeout 1s ifconfig
timeout 1s sudo route -n    # see our gateway
timeout 1s bridge link
timeout 1s bridge fdb
echo

echo "Network configuration script: Bridge"
BRIDGE=mg_bridge0
BRIDGE_IP=192.168.32.1
BRIDGE_MASK=255.255.255.0
BRIDGE_IP_MASK=192.168.32.0/24
BRIDGE_BROADCAST=192.168.32.255
PHY=eth0
TAP=tap0

sudo ip link add $BRIDGE type bridge	# Create brige
sudo ifconfig $BRIDGE $BRIDGE_IP netmask $BRIDGE_MASK up
echo "Network configuration script: TAP"

echo "Create $TAP"
sudo ip tuntap add dev $TAP mode tap	# Create tuntap
sudo ip link set $TAP master $BRIDGE	# Link tap-bridge
sudo ip link set $TAP up
echo "Network configuration script: NAT"

sudo iptables -A FORWARD -d $BRIDGE_IP_MASK -o $BRIDGE -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT
sudo iptables -A FORWARD -s $BRIDGE_IP_MASK -i $BRIDGE -j ACCEPT
sudo iptables -A FORWARD -i $BRIDGE -o $BRIDGE -j ACCEPT
sudo iptables -A FORWARD -o $BRIDGE -j REJECT --reject-with icmp-port-unreachable
sudo iptables -A FORWARD -i $BRIDGE -j REJECT --reject-with icmp-port-unreachable
sudo iptables -t nat -A POSTROUTING -s $BRIDGE_IP_MASK -d 224.0.0.0/24 -j RETURN
sudo iptables -t nat -A POSTROUTING -s $BRIDGE_IP_MASK -d 255.255.255.255/32 -j RETURN
sudo iptables -t nat -A POSTROUTING -s $BRIDGE_IP_MASK ! -d $BRIDGE_IP_MASK -p tcp -j MASQUERADE --to-ports 1024-65535
sudo iptables -t nat -A POSTROUTING -s $BRIDGE_IP_MASK ! -d $BRIDGE_IP_MASK -p udp -j MASQUERADE --to-ports 1024-65535
sudo iptables -t nat -A POSTROUTING -s $BRIDGE_IP_MASK ! -d $BRIDGE_IP_MASK -j MASQUERADE
echo 1 | sudo tee /proc/sys/net/ipv4/ip_forward

# Do we have connectivity ?
echo "Check connectivity:"
wget https://cesanta.com/robots.txt
echo robots.txt:
cat  robots.txt
rm   robots.txt
echo

# Confirm OK
echo "Done:"
timeout 1s ifconfig       
timeout 1s sudo route -n
timeout 1s bridge fdb
timeout 1s bridge link