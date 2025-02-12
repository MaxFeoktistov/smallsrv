rem Example of script that server call when connection estabilished
rem Arguments:
rem <interface> <ip> <netmask> <gw> '<dns servers>'

echo ON
netsh interface ipv4 set address name="%1" static %2 %3 %4
rem netsh interface ipv4 set address name="Local Area Connection 4" static 192.168.111.1 255.255.255.0 192.168.111.1

rem netsh interface ipv4 add dnsservers %1 address=8.8.8.8 index=1

