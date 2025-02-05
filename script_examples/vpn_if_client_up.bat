rem Example of script that server call when connection estabilished
rem Arguments:
rem <interface> <ip> <netmask> <gw> "<dns servers>" <ip_of_remote_vpn_server>

echo ON

rem !!!Drect yuor original gateway in next line:!!!
rem set ORIGINAL_GW=192.168.1.1

netsh interface ipv4 set address name=%1 static %2 %3 %4

echo if:%1 ip:%2 mask:%3 gw:%4 dns:%5 remote:%6

rem !!! Uncomment next 4 lines to redirect all IP4 trafic to VPN !!!
rem route ADD %6 MASK 255.255.255.255 %ORIGINAL_GW%
rem route DELETE 0.0.0.0 MASK 0.0.0.0 %ORIGINAL_GW%
rem route ADD 0.0.0.0 MASK 0.0.0.0 %4
rem netsh interface ipv4 add dnsserver %1 address=8.8.8.8 index=1


