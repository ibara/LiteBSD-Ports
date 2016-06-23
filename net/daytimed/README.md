daytimed
========
standalone daytime protocol server

`daytimed` is a standalone server which implements the daytime protocol
as specified in RFC 867. It takes listens on TCP and, optionally, UDP
port 13.

`daytimed` returns the date and time in a format identical to the
date(1) command on OpenBSD executed without options.

`daytimed` is useful as a debugging and measurement tool, and for
testing connections.

Installation
------------
    $ make
    # make install

You then need to add a `_daytimed` user to your system. You can do this
by executing `vipw` and adding the following line to your `/etc/passwd`:

    _daytimed:*:50:50::0:0:Daytime Daemon:/var/empty:/sbin/nologin

Change the user id and group id (50:50) to a different value if 50 is
already taken.

Usage
-----
The following commands provide examples of how to connect to `daytimed`.

Assuming `daytimed` is listening on localhost, this example connects
via TCP:

    $ telnet localhost 13

And this example connects via UDP:

    $ echo "test" | nc -u -w1 localhost 13

UDP access requires the `-u` flag to be passed to `daytimed` at startup.

License
-------
`daytimed` is licensed under the ISC license. See LICENSE for details.

Get a tarball
-------------
http://devio.us/~bcallah/daemons/daytimed-20160704.tar.gz
