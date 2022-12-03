
IMAGE_NAME = nginx_img
CONTAINER_NAME = nginx_con

all: build run

build: 
	sudo docker build -t $(IMAGE_NAME) .

run:
	sudo docker run -it -d -p 2056:2056 --name=$(CONTAINER_NAME) $(IMAGE_NAME)

exec:
	sudo docker exec -it $(CONTAINER_NAME) bash

stop:
	sudo docker stop $(CONTAINER_NAME)

rm:
	sudo docker rm $(CONTAINER_NAME)


clean: stop rm

fclean: clean
	sudo docker image rm $(IMAGE_NAME)

re: clean all

