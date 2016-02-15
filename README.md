LiteBSD Ports
=============
The LiteBSD ports tree is a collection of software that acts as a
third-party repository. It functions just like the ports tree does
on the big BSDs: it creates packages that can then be installed by
the LiteBSD `opkg(1)` tool.

Usage
-----
On the machine you use to build LiteBSD:
```
$ cd $BSDSRC
$ git clone https://github.com/ibara/LiteBSD-Ports.git ports
```
Then `cd` into the directory containing the software you want to
install.

To create a package that you will later install with the
`opkg(1)` utility on a running LiteBSD machine:
```
$ bmake package
```
The package will be in `${.CURDIR}/work/`.

License
-------
All included software carries its own license. Please see each
individual port for its license terms.

As far as anything in the LiteBSD ports tree infrastructure can
be considered copyrightable, it is hereby released by the author
into the public domain. In the event that public domain is
unacceptable, it is released under the terms of the MIT License
with the following header:

`Copyright (c) 2015-2016 Brian Callahan <bcallah@devio.us>`

