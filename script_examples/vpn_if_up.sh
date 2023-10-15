#!/bin/bash

# Example of script that server call when connection estabilished
# Arguments:
# <interface> <ip> <netmask> <gw> '<dns servers>' <ip_of_vpn_server>

echo -e "\n\nVPN interface UP scrpt\n"

if [[ $# -gt 5 ]] ; then
  vpn_server=$6
fi


echo "$1 $2 $3 gw:$4 dns:$5 ip_of_vpn_server:$vpn_server"

sleep 0.5

exit 0

