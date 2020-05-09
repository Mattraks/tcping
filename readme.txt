Installation:
------------
make install


Usage:
-----
tcping -h hostname
(uses port 80) or
tcping -h hostname -p port

ping once:
tcping -h host -p port -c 1

tcping Return code:
0 means can connect (at least once)
2 if the host or service could not be resolved
127 means unable to connect at all

examples:
tcping -h 127.0.0.1 -p 8080

Please support my opensource development: http://www.vanheusden.com/wishlist.php
