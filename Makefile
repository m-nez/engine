.PHONY: test

.DEFAULT: test

test:
	cd build && make
	cp build/demo demo
	./demo

clean:
	cd build && make clean
