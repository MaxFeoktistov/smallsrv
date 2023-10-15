rem Example of script that server call when connection estabilished
rem Arguments:
rem <interface> <ip> <netmask> <gw> "<dns servers>" <ip_of_remote_vpn_server>

echo ON

rem Drect yuor original gateway in next line:
set ORIGINAL_GW=10.0.0.1

netsh interface ipv4 set address name=%1 static %2 %3 %4

rem netsh interface ipv4 set address name="Local Area Connection 4" static 192.168.111.1 255.255.255.0 192.168.111.1

echo if:%1 ip:%2 mask:%3 gw:%4 dns:%5 remote:%6

rem Uncomment next 3 lines to redirect all IP4 trafic to VPN
rem route ADD %5 MASK 255.255.255.255 %ORIGINAL_GW%
rem route ADD 0.0.0.0 MASK 0.0.0.0 %3
rem route DELETE 0.0.0.0 MASK 0.0.0.0 %ORIGINAL_GW%

