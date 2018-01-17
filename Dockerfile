FROM alpine:edge as BUILD
RUN apk update && apk add build-base
COPY nsenter1.c ./
RUN cc -Wall -static nsenter1.c -o /usr/bin/nsenter1

FROM scratch
COPY --from=BUILD /usr/bin/nsenter1 /usr/bin/nsenter1
ENTRYPOINT ["/usr/bin/nsenter1"]
