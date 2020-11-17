Installation:
------------
make install


Usage
tcping [timeout] <destination>

Options:
  <destination>      dns name or ip address
  -c <count>         count how many times to connect
  -f                 flood connect (no delays)
  -h                 print help and exit
  -i <interval>      interval delay between each connect (e.g. 1)
  -p <port>          portnr portnumber (e.g. 80)
  -q                 quiet, only returncode
  -t <timeout>       time to wait for response (e.g. 3)

tcping Return code:
0 means can connect (at least once)
2 if the host or service could not be resolved
3 No hostname given or ip
127 means unable to connect at all

examples:
tcping -p 8080 127.0.0.1

Please support my opensource development: http://www.vanheusden.com/wishlist.php
