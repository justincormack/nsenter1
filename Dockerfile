FROM scratch
COPY /usr/bin/nsenter1 /usr/bin/nsenter1
ENTRYPOINT ["/usr/bin/nsenter1"]
