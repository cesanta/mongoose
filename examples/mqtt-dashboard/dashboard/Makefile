PWD := $(shell pwd)
NPX ?= npx

all: bundle.js main.css
	make -C ../../http-server ARGS="-d $(PWD)"

# Bundle JS libraries (preact, preact-router, ...) into a single file
bundle.js:
	curl -s https://npm.reversehttp.com/preact,preact/hooks,htm/preact,preact-router -o $@

# Create optimised CSS. Prerequisite: npm -g i tailwindcss tailwindcss-font-inter
main.css: index.html $(wildcard *.js)
	$(NPX) tailwindcss -o $@ --minify

clean:
	true
