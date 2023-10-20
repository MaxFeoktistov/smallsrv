#!/bin/bash

# Example of script that server call when connection estabilished
# Arguments:
# <interface> <ip> <netmask> <gw> '<dns servers>' <ip_of_vpn_server>

echo -e "\n\nVPN client interface UP scrpt\n"

ORIGINAL_GW=$(route -n | awk '/^0.0.0.0/ {print $1 }')

if [[ $# -gt 5 ]] ; then
  vpn_server=$6
fi


echo "if:$1 ip:$2 mask:$3 gw:$4 dns:$5 ip_of_vpn_server:$vpn_server usnet:$USNET wan_if:$WAN_IF"

# Uncomment next 3 lines to redirect all IP4 trafic to VPN
# route add -host $6 gw $ORIGINAL_GW
# route del default
# route add default gw %3

# Uncomment next lines to update your DNS
# if [[ -n $5 ]] ; then
#   DNS=${5//[,;]/ }
#   if [[ ! -e /etc/resolv.conf.orig ]] ; then
#     mv /etc/resolv.conf /etc/resolv.conf.orig
#   fi
#   for i in $DNS ; do
#     echo "nameserver $i"
#   done  > /etc/resolv.conf
# fi


exit 0

