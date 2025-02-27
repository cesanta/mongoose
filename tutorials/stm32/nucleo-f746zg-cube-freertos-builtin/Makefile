BOARD = f746
IDE = CubeIDE
RTOS = FreeRTOS
WIZARD_URL ?= http://mongoose.ws/wizard

TARGET ?= Debug
DOCKER = docker run --rm -v $(CURDIR):/root -w /root
IMAGE ?= scaprile/cubeozone
# set PATHTO if image author did not set path

all build example: firmware.elf

firmware.elf: wizard
	mkdir -p workspace
	PROJNAME=`xq -r .projectDescription.name wizard/.project` && \
	$(DOCKER) $(IMAGE) $(PATHTO)headless-build.sh -data workspace -import wizard -cleanBuild $$PROJNAME/$(TARGET) && \
	cp wizard/$(TARGET)/$$PROJNAME.elf firmware.elf

wizard:
	hash=$$(curl -s -X POST -H "Content-Type: application/json" -d '{"build":{"board":"$(BOARD)","ide":"$(IDE)","rtos":"$(RTOS)"}}' $(WIZARD_URL)/api/hash | jq -r '.hash') \
	&& curl -s $(WIZARD_URL)/api/zip/$(BOARD)/$(IDE)/$(RTOS)/$$hash -o wizard.zip
	unzip wizard.zip
	cd wizard/Core/Src ; rm mongoose.c ; cp -rL ../../../../../../mongoose.c .
	cd wizard/Core/Inc ; rm mongoose.h ; cp -rL ../../../../../../mongoose.h .


clean:
	rm -rf firmware.* wizard* workspace
