IMAGE_NAME=gcr.io/radical-sloth/how-angry-is-the-internet
VERSION=0.0.1


.PHONY: build
build:
	@docker build -t $(IMAGE_NAME):$(VERSION) .

.PHONY: run
run:
	@docker run -p 5000:5000 $(IMAGE_NAME):$(VERSION)

.PHONY: console
console:
	@docker run -it $(IMAGE_NAME):$(VERSION) -c "bash"

.PHONY: train
train:
	@docker run -v $(shell pwd)/./src:/code/ $(IMAGE_NAME):$(VERSION) train.py

.PHONY: push
push:
	@docker push $(IMAGE_NAME):$(VERSION)

.PHONY: deploy
deploy:
	@make build
	@make push
	@gcloud run deploy internet-angryometer-server \
		--image $(IMAGE_NAME):$(VERSION) \
		--region us-central1 \
		--port 5000 \
		--concurrency 80 \
		--cpu 1 \
		--max-instances 1 \
		--timeout 5 \
		--memory 128Mi \
		--platform managed \
		--allow-unauthenticated \
		--project radical-sloth
