default:
	docker build -t nsenter1:build -f Dockerfile.build .
	docker run nsenter1:build | docker build -t justincormack/nsenter1 -
