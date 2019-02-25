# nsenter1

[![Docker Build Status](https://img.shields.io/docker/pulls/justincormack/nsenter1.svg)](justincormack/nsenter1)

Minimal image for `nsenter` to namespaces of PID 1

* mnt
* uts
* net
* ipc

To achieve the above with the basic alpine image you would enter:

    $ docker run -it --rm --privileged --pid=host alpine:edge nsenter -t 1 -m -u -n -i sh
    / #

Unfortunately, however, there is an [outstanding issue](https://github.com/gliderlabs/docker-alpine/issues/359)
that prevents specifying the target pid.

With this image, you can simply run the following:

    $ docker run -it --rm --privileged --pid=host justincormack/nsenter1
    / #

## So what is this good for

`nsenter` allows you to enter a shell in a running container (technically into the namespaces that provide
a container's isolation and limited access to system resources). The crazy thing is that this image allows
you to run a privileged container that runs nsenter for the process space running as pid 1. How is this useful?

Well, this is useful when you are running a lightweight, container-optimized Linux distribution such as
[LinuxKit](https://blog.docker.com/2017/04/introducing-linuxkit-container-os-toolkit/).
Here is one simple example: say you want to teach a few people about Docker networking and you want to
show them how to inspect the default bridge network after starting two containers using `ip addr show`;
the problem is if you are demonstrating with Docker for Mac, for example, your containers are not running on
your host directly, but are running instead inside of a minimal Linux OS virtual machine specially built for
running containers, i.e., LinuxKit. But being a lightweight environment, LinuxKit isn't running `sshd`, so
how do you get access to a shell so you can run `nsenter` to inspect the namespaces for the process running as pid 1?

Well, you could run the following:

    $ screen ~/Library/Containers/com.docker.docker/Data/vms/0/tty

Docker for Mac does expose a screen session to attach to, but it's a bit less than ideal if you're not familiar
with screen. It's not a big deal, but it's not optimal and it's also very specific to Docker for Mac. Since
we're already running Docker the general solution is ideal in this case:

```
$ docker run -it --rm --privileged --pid=host justincormack/nsenter1
/ # ip a
256: vethb72bfa3@if255: <BROADCAST,MULTICAST,UP,LOWER_UP,M-DOWN> mtu 1500 qdisc noqueue master docker0 state UP
    link/ether 7a:41:32:02:63:7c brd ff:ff:ff:ff:ff:ff
    inet6 fe80::7841:32ff:fe02:637c/64 scope link
       valid_lft forever preferred_lft forever
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN qlen 1
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 brd 127.255.255.255 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host
       valid_lft forever preferred_lft forever
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP qlen 1000
    link/ether 02:50:00:00:00:01 brd ff:ff:ff:ff:ff:ff
    inet 192.168.65.3/24 brd 192.168.65.255 scope global eth0
       valid_lft forever preferred_lft forever
    inet6 fe80::49e8:1c10:4c64:c980/64 scope link
       valid_lft forever preferred_lft forever
...
```

Have fun!
 
