example:
	true

build:
	sh .zephyr_install.sh
	make -C http-client build
	make -C http-server build
	make -C mqtt-aws-client build
	make -C websocket-server build

clean:
	rm -rf */*/mongoose.*
