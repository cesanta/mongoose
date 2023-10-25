# make pre, run STM32PackCreator, make post

pre:
	find Examples/x/x/* -maxdepth 0 -exec mkdir -p {}"/Middlewares/Third_Party/Cesanta_Network Library/Mongoose/" \;
	find Examples/x/x/* -maxdepth 0 -exec cp ../../mongoose.[ch] {}"/Middlewares/Third_Party/Cesanta_Network Library/Mongoose/" \;
	grep -l net.h Examples/x/x/*/Core/Src/main.c | xargs -n 1 dirname | sed 's.$$./.' | xargs -n 1 cp ../../examples/device-dashboard/net.c
	grep -l net.h Examples/x/x/*/Core/Src/main.c | xargs -n 1 dirname | sed 's.$$./.' | sed 's/Src/Inc/' | xargs -n 1 cp ../../examples/device-dashboard/net.h
	grep -l net.h Examples/x/x/*/Core/Src/main.c | xargs -n 1 dirname | sed 's.$$./.' | xargs -n 1 cp ../../examples/device-dashboard/packed_fs.c
	# examples can't be tested in place anymore because of that nasty relative firmware path

post:
	rm -f Examples/x/x/*/Middlewares/Third_Party/Cesanta_Network\ Library/Mongoose/mongoose.[ch]
	rm -f Examples/x/x/*/Core/Src/net.c
	rm -f Examples/x/x/*/Core/Inc/net.h
	rm -f Examples/x/x/*/Core/Src/packed_fs.c

packcreator:
	/opt/st/STM32CubeMX/jre/bin/java -jar /opt/st/STM32CubeMX/utilities/STM32PackCreator/STM32PackCreator.jar

clean:
	rm -rf Files
	rm *.pack


# update MX info in examples
updateexamples:
	find Examples/ -name '.mxproject' -exec sed -i 's/$(OLDREL)/$(NEWREL)/g' {} \; && \
	find Examples/ -name '*.ioc' -exec sed -i 's/$(OLDREL)/$(NEWREL)/g' {} \;
	


# Test examples included in new pack to be released
testexamples: unpack
	$(MAKE) -f ../../test/cube/Makefile "PROJECTS=$(wildcard packcontents/Projects/*/Examples/*)"
	rm -rf packcontents

unpack: FORCE
	unzip -x Cesanta.I-CUBE-Mongoose.*.pack -d packcontents

FORCE:
