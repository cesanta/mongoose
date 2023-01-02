NAME = raspberry-pi-pico-w

$(NAME):
	git clone --depth 1 https://github.com/mongoose-examples/$(NAME)

build: | $(NAME)
	cp ./../../../mongoose.[ch] $(NAME)
	cp ./../../device-dashboard/packed_fs.c $(NAME)
	cp ./../../device-dashboard/net.c $(NAME)
	make -C $(NAME) build

clean:
	make -C $(NAME) clean
	rm -rf $(NAME)
