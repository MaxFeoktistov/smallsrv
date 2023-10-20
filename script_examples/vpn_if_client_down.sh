#!/bin/bash

# Example of script that server call when connection estabilished
# Arguments:
# <interface> <ip> <ip_of_vpn_server>

echo -e "\n\nVPN client interface DOWN scrpt\n"

CURENT_GW_IF=$(route -n | awk '/^0.0.0.0/ {print $8 }')
ORIGINAL_GW=$(route -n | awk "/^$2/ {print \$1 }" )

if [[ $# -gt 2 ]] ; then
  vpn_server=$3
fi


echo "if:$1 ip:$2 ip_of_vpn_server:$vpn_server ORIGINAL_GW:$ORIGINAL_GW CURENT_GW_IF:$CURENT_GW_IF"

# Uncomment next lines to return back all IP4 trafic to original gateway
# if [[ -n "$ORIGINAL_GW" && $CURENT_GW_IF == $1 ]] ; then
# route del default
# route add default gw $ORIGINAL_GW
# fi

# Uncomment next lines to return back your DNS
# if [[ -e /etc/resolv.conf.orig ]] ; then
#   mv -f /etc/resolv.conf.orig /etc/resolv.conf
# fi


exit 0

