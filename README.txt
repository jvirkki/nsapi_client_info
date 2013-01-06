
Implements a minimal NSAPI SAF which simply writes out the IP address
of the client making the request.

This is suitable for use with compatible web servers such as heliod.
See http://sourceforge.net/projects/heliod/


Building
========

Set the environment variable NSAPI_INCLUDE_DIR to point at the
directory on your system where the public nsapi.h include file is
installed. Then just run make.

% NSAPI_INCLUDE_DIR=$PATH_TO_DIR_CONTAINING_NSAPI_H make

Copy client_info.so to a convenient stable location from where heliod
can load it.


Configuring
===========

Load the shares library from magnus.conf (substitute $SOPATH below to
the directory where you installed client_info.so):

Init fn=load-modules shlib=$SOPATH/client_info.so funcs=print_client_info

Then, arrange to call print_client_info as desired in obj.conf.

As an example, the lines below map "/myip" on the server to this function:

NameTrans fn=assign-name from="/myip" name="get-client-info"

...

<Object name="get-client-info">
Service fn="print_client_info"
</Object>

