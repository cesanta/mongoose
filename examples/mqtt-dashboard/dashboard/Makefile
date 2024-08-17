all:
	make -C ../../../tutorials/http/http-server ARGS="-d $(CURDIR)"

# Bundle JS libraries (preact, preact-router, ...) into a single file
bundle.js:
	curl -s https://npm.reversehttp.com/preact,preact/hooks,htm/preact,preact-router -o $@

# Create optimised CSS. Prerequisite: npm -g i tailwindcss tailwindcss-font-inter
NPX ?= npx
main.css: index.html $(wildcard *.js)
	$(NPX) tailwindcss -o $@ --minify

clean:
	true
