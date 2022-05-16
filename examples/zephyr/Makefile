example:
	true

localbuild:
	sh .zephyr_install.sh
	make -C http-client localbuild
	make -C http-server localbuild
	make -C mqtt-aws-client localbuild
	make -C websocket-server localbuild

clean:
	rm -rf */*/mongoose.*