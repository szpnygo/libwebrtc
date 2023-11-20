.PHONY: docker docker-build

docker-build:
	docker build -t neosu/libwebrtc-build:v1 .

docker:
	docker run --net=host -e http_proxy=http://host.docker.internal:7890 -e https_proxy=http://host.docker.internal:7890 --rm -it -v "${PWD}:/app" -w /app neosu/libwebrtc-build:v1 /bin/sh