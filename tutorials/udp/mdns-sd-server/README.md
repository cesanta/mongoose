```sh
$ avahi-browse -r  _myservice._tcp
+ virbr0 IPv4 Mongoose                                      _myservice._tcp      local
= virbr0 IPv4 Mongoose                                      _myservice._tcp      local
   hostname = [Mongoose.local]
   address = [192.168.69.11]
   port = [9876]
   txt = []
```

```sh
$ avahi-browse -r  _myservice._udp
+ virbr0 IPv4 Mongoose                                      _myservice._udp      local
= virbr0 IPv4 Mongoose                                      _myservice._udp      local
   hostname = [Mongoose.local]
   address = [192.168.69.11]
   port = [9876]
   txt = ["{\"property\": \"value\"}"]
```
