# make pre, run STM32PackCreator, make post

pre:
	find Examples/* -maxdepth 0 -exec mkdir -p {}"/Middlewares/Third_Party/Cesanta_Network Library/Mongoose/" \;
	find Examples/* -maxdepth 0 -exec cp ../../mongoose.[ch] {}"/Middlewares/Third_Party/Cesanta_Network Library/Mongoose/" \;
	grep -l net.h Examples/*/Core/Src/main.c | xargs -n 1 dirname | sed 's.$$./.' | xargs -n 1 cp ../../examples/device-dashboard/net.c
	grep -l net.h Examples/*/Core/Src/main.c | xargs -n 1 dirname | sed 's.$$./.' | sed 's/Src/Inc/' | xargs -n 1 cp ../../examples/device-dashboard/net.h
	grep -l net.h Examples/*/Core/Src/main.c | xargs -n 1 dirname | sed 's.$$./.' | xargs -n 1 cp ../../examples/device-dashboard/packed_fs.c
	$(MAKE) -f ../../test/cube/Makefile "PROJECTS=$(wildcard Examples/*)"
	sudo rm -rf Examples/*/Debug
	sudo rm -rf Examples/*/Release

post:
	rm -f Examples/*/Middlewares/Third_Party/Cesanta_Network\ Library/Mongoose/mongoose.[ch]
	rm -f Examples/*/Core/Src/net.c
	rm -f Examples/*/Core/Inc/net.h
	rm -f Examples/*/Core/Src/packed_fs.c

packcreator:
	/opt/st/STM32CubeMX/jre/bin/java -jar /opt/st/STM32CubeMX/utilities/STM32PackCreator/STM32PackCreator.jar

clean:
	rm -rf Files
	rm *.pack


# Test examples included in new pack to be released
testexamples: unpack
	$(MAKE) -f ../../test/cube/Makefile "PROJECTS=$(wildcard Projects/*/Examples/*)"
	rm -rf Projects

unpack: FORCE
	unzip -x Cesanta.I-CUBE-Mongoose.*.pack Projects/*

FORCE:
