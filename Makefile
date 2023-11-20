.PHONY: docker

docker:
	docker run --rm -it -v "${PWD}:/app" -w /app buildtest:v1 /bin/sh