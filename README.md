LiteBSD Ports
=============
The LiteBSD ports tree is a collection of software that acts as a
third-party repository. It functions just like the ports tree does
on the big BSDs: it creates packages that can then be installed by
the LiteBSD `pkg(1)` tool or installed directly to your LiteBSD
sdcard.img using a built-in minimal version of the `pkg(1)` tool.

Usage
-----
On the machine you use to build LiteBSD:
```
$ cd $BSDSRC
$ git clone https://github.com/ibara/LiteBSD-Ports.git
```
Then `cd` into the directory containing the software you want to
install.

If you want to create a package that you will later install on a
running LiteBSD machine:
```
$ make package
```
The package will be in the `work/` directory.

If you want to install the package direct for inclusion on your
pre-build LiteBSD sdcard.img:
```
$ make install
```

License
-------
All software carries its own license. Please see each individual
port for the license terms.

As far as anything in the LiteBSD ports tree infrastructure can
be considered copyrightable, it is hereby released by the author
into the public domain. In the event that public domain is
unacceptable, it is released under the terms of the MIT License
with the following header:

`Copyright (c) 2015 Brian Callahan <bcallah@devio.us>`

