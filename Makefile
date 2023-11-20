.PHONY: docker

docker:
	docker run --net=host -e http_proxy=http://host.docker.internal:7890 -e https_proxy=http://host.docker.internal:7890 --rm -it -v "${PWD}:/app" -w /app buildtest:v1 /bin/sh