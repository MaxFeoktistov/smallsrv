#!/bin/bash

# Example of script that server call when server VPN interface up
# Arguments:
# <interface> <ip> <netmask> <gw>

echo -e "\n\nVPN interface UP scrpt\n"

WAN_IF=$(route -n | awk '/^0.0.0.0/ {print $8 }')
USNET=$(perl -e '$a=pack("C4",split(/[.]/,$ARGV[0])) & ($m=pack("C4",split(/[.]/,$ARGV[1]))); $m=unpack("L",$m); for($i=0;$m;$i++){$m=$m>>1;} printf "%u.%u.%u.%u/$i", unpack("C4",$a); ' $2 $3)

echo "if:$1 ip:$2 mask:$3 gw:$4 usnet:$USNET wan_if:$WAN_IF"

# Uncoment next lines to share internet to VPN users:
# echo "1" > /proc/sys/net/ipv4/ip_forward
# if ! iptables -t nat -L -v -n | grep $USNET then
# iptables -t nat -A POSTROUTING  -o $WAN_IF -s $USNET -j MASQUERADE
# fi


# Uncoment next lines to create common bridge with VPN TAP and your LAN interface (for TAP interface only!):
# LAN_IF=eth1
# LAN_IP=$2
# LAN_MASK=$3
# BR=br0
# ifconfig $1 0.0.0.0
# if ! grep $BR /proc/net/dev ; then
# brctl addbr $BR
# ifconfig $LAN_IF 0.0.0.0
# ifconfig  $BR  $LAN_IP netmask $LAN_MASK
# brctl addif $BR $LAN_IF
# fi
# brctl addif $BR $1


exit 0

