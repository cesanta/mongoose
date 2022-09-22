NAME ?= $(notdir $(CURDIR))

$(NAME):
	git clone --depth 1 https://github.com/mongoose-examples/$(NAME)

build: | $(NAME)
	cp ./../../../mongoose.c $(NAME)/Core/Src/
	cp ./../../../mongoose.h $(NAME)/Core/Inc/
	make -C $(NAME) build

clean:
	rm -rf $(NAME)

flash:
	make -C $(NAME) flash

.PHONY: flash

