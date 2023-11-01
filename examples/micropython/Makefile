
all: example
	micropython/ports/unix/build-standard/micropython main.py

example: micropython
	$(MAKE) -C micropython/ports/unix submodules
	$(MAKE) -C micropython/ports/unix USER_C_MODULES=../../..

micropython:
	git clone https://github.com/micropython/micropython.git

clean:
	rm -rf micropython
