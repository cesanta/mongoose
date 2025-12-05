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
	hash=$$(curl -H 'Cookie: mongoose_wizard_uid=F2PPlAgQCqSthV1g' -s -X POST -H "Content-Type: application/json" -d '{"build":{"board":"$(BOARD)","ide":"$(IDE)","rtos":"$(RTOS)"},"http":{"http": true}}' $(WIZARD_URL)/api/hash | jq -r '.hash') \
	&& curl -H 'Cookie: mongoose_wizard_uid=F2PPlAgQCqSthV1g' -s $(WIZARD_URL)/api/zip -o wizard.zip
	unzip wizard.zip
	cd wizard/mongoose ; rm mongoose.[ch] ; cp -L ../../../../../mongoose.c . ; cp -L ../../../../../mongoose.h .


clean:
	rm -rf firmware.* wizard* workspace
