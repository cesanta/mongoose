NAME  = ti-ek-tm4c1294xl-http-server
DOCKER ?= docker run -v $(CURDIR):/workspace mdashnet/ccs

build:
	git clone --depth 1 https://github.com/mongoose-examples/$(NAME)
	cp ./../../../mongoose.[ch] $(NAME)/
	$(DOCKER) /opt/ti/ccs/eclipse/eclipse -noSplash -data /workspace -application com.ti.ccstudio.apps.projectBuild -ccs.autoImport -ccs.projects $(NAME) -ccs.configuration Release

clean:
	rm -rf $(NAME)
