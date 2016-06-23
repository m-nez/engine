.PHONY: test

test:
	cd build && make
	cp build/demo demo
	./demo

