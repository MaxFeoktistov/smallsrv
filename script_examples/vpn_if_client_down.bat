rem Example of script that server call when VPN connection closed
rem Arguments:
rem <interface> <ip>

echo ON

rem !!! Drect yuor original gateway in next line:
rem set ORIGINAL_GW=192.168.12.5

echo if:%1 ip:%2

rem Uncomment next 2 lines to direct all IP4 trafic to original GW
rem route DELETE 0.0.0.0
rem route ADD 0.0.0.0 MASK 0.0.0.0 %ORIGINAL_GW%

