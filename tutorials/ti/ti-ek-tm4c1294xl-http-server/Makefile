NAME  = ti-ek-tm4c1294xl-http-server
DOCKER ?= docker run --rm $(DOCKER_ARGS) -v $(CURDIR):/workspace mdashnet/ccs

$(NAME):
	git clone --depth 1 https://github.com/mongoose-examples/$(NAME)

build: | $(NAME)
	cp ./../../../mongoose.[ch] $(NAME)/
	$(DOCKER) /opt/ti/ccs/eclipse/eclipse -noSplash -data /workspace -application com.ti.ccstudio.apps.projectBuild -ccs.autoImport -ccs.projects $(NAME) -ccs.configuration Release

clean:
	test -d $(NAME) && $(DOCKER) rm -rf /workspace/$(NAME) || true

flash:	DOCKER_ARGS = -it --privileged -v /dev/bus/usb/:/dev/bus/usb -w /workspace
flash:
	$(DOCKER) /opt/ti/ccs/ccs_base/scripting/examples/loadti/loadti.sh -c $(NAME)/ek-tm4c1294xl.ccxml $(NAME)/Release/$(NAME).out 

