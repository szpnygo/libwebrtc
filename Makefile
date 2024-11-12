.PHONY: docker docker-build

docker-build:
	docker buildx build --platform linux/amd64 -t neosu/libwebrtc-build:v2 .

docker:
	docker run --platform linux/amd64 --net=host -e http_proxy=http://host.docker.internal:7890 -e https_proxy=http://host.docker.internal:7890 --rm -it -v "${PWD}:/app" -w /app neosu/libwebrtc-build:v2 /bin/sh