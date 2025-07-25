all:
	make -C ../../../http/http-server ARGS="-d $(CURDIR)"

# Bundle JS libraries (preact, preact-router, ...) into a single file
bundle.js:
	curl -s https://npm.reversehttp.com/preact,preact/hooks,htm/preact,preact-router,@preact/signals-core,@preact/signals -o $@

clean:
	true
