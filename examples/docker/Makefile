# run 'make docker'

all example clean:
	true
	
docker:
	mkdir -p web_root
	test -f web_root/index.html || echo "<HTML><HEAD/><BODY><H1>OK</H1></BODY></HTML>" > web_root/index.html 
	docker image inspect mongoose --format="image exists" || docker build -t mongoose .
	docker run -it --rm -v $(CURDIR):/workdir -p 8000:8000 -w /workdir mongoose /mongoose/examples/http-server/server -d web_root

.PHONY: docker

