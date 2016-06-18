FILES= \
	   __init__.py \
	   VBBExport.py

VBB.zip: $(FILES)
	mkdir -p VBB
	cp $^ VBB
	zip $@ -r VBB

.PHONY: clean

clean:
	rm -f VBB.zip
