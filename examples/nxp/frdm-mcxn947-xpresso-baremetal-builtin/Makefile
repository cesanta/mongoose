BOARD = mcxn947
IDE = MCUXpresso
RTOS = baremetal
WIZARD_URL ?= http://mongoose.ws/wizard

TARGET ?= Debug
DOCKER = docker run --rm -v $(CURDIR):/root -w /root
IMAGE ?= scaprile/xpresso
HEADLESS_BUILD = /usr/local/mcuxpressoide/ide/mcuxpressoide --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild

all build example: firmware.axf

firmware.axf: wizard
	mkdir -p workspace
	PROJNAME=`xq -r .projectDescription.name wizard/.project` && \
	($(DOCKER) $(IMAGE) $(HEADLESS_BUILD) -data workspace -import wizard -cleanBuild $$PROJNAME/$(TARGET) || true) && \
	cp wizard/$(TARGET)/$$PROJNAME.axf firmware.axf

wizard:
	hash=$$(curl -s -X POST -H "Content-Type: application/json" -d '{"build":{"board":"$(BOARD)","ide":"$(IDE)","rtos":"$(RTOS)"}}' $(WIZARD_URL)/api/hash | jq -r '.hash') \
	&& curl -s $(WIZARD_URL)/api/zip/$(BOARD)/$(IDE)/$(RTOS)/$$hash -o wizard.zip
	unzip wizard.zip
	cd wizard/source ; rm mongoose.[ch] ; cp -rL ../../../../../mongoose.[ch] .


clean:
	sudo rm -rf firmware.* wizard* workspace mcuxpresso .cache .eclipse .p2
