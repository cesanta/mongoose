NAME ?= $(notdir $(CURDIR))

build:
	git clone --depth 1 https://github.com/mongoose-examples/$(NAME)
	make -C $(NAME) build

clean:
	rm -rf $(NAME)
