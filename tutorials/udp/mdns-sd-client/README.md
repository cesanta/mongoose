
Browses for `_http._tcp.local` services on the local network using mDNS/DNS-SD.

```sh
make
cc main.c mongoose.c        -W -Wall -Wextra -g -I.   -DMG_ENABLE_LINES  -o example                                    
./example                    
1304aa594 3 net.c:311:mg_mgr_init       MG_IO_SIZE: 16384, TLS: none
1304aa594 3 net.c:224:mg_listen         1 4 udp://224.0.0.251:5353
1304aa594 3 sock.c:167:mg_send          1 4 0:0:0 34 err 0
1304aa594 3 sock.c:361:read_conn        1 4 0:0:0 34 err 0
1304aa594 3 dns.c:478:handle_mdns_query PTR request for _http._tcp
1304aa5d0 3 sock.c:361:read_conn        1 4 0:0:0 289 err 0
1304aa5d1 3 dns.c:703:handle_mdns_respo PTR response for _http._tcp.local from 3D03F3CPAU00096.local
1304aa5d1 2 main.c:15:mdns_ev_handler   Got a response
	SERVICE -> _http._tcp
	NAME -> 3D03F3CPAU00096.local
	PORT -> 80
	TXT -> host=3D03F3CPAU00096mac=e0:50:8b:8d:2f:75ip=192.168.69.220
	URL -> http://3D03F3CPAU00096.local:80

1304aa622 3 sock.c:361:read_conn        1 4 0:0:0 226 err 0
1304aa622 3 dns.c:703:handle_mdns_respo PTR response for _http._tcp.local from DIP-3TIR3020140717CCWR473261824.local
1304aa622 2 main.c:15:mdns_ev_handler   Got a response
	SERVICE -> _http._tcp
	NAME -> DIP-3TIR3020140717CCWR473261824.local
	PORT -> 80
	ADDRESS -> 192.168.69.225
	TXT -> path=/
	URL -> http://192.168.69.225:80

1304aa6f5 3 sock.c:361:read_conn        1 4 0:0:0 169 err 0
1304aa6f5 3 dns.c:703:handle_mdns_respo PTR response for _http._tcp.local from PHILIPS_HD9255_3_6B7E5E.local
1304aa6f5 2 main.c:15:mdns_ev_handler   Got a response
	SERVICE -> _http._tcp
	NAME -> PHILIPS_HD9255_3_6B7E5E.local
	PORT -> 80
	ADDRESS -> 192.168.69.167
	TXT -> Device Type=Domestic Appliance
	URL -> http://192.168.69.167:80

```

