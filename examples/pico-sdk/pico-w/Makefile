NAME = raspberry-pi-pico-w

$(NAME):
	git clone --depth 1 https://github.com/mongoose-examples/$(NAME)

build: | $(NAME)
	cp ./../../../mongoose.[ch] $(NAME)
	cp ./../../device-dashboard/packed_fs.c $(NAME)
	cp ./../../device-dashboard/net.[ch] $(NAME)
	make -C $(NAME) build

clean:
	rm -rf $(NAME)
