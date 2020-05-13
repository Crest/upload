CFLAGS = -O2 -Weverything -Werror

.PHONY: clean

all: upload

upload: upload.c
	cc $(CFLAGS) -o $@ $>
	strip $@

clean:
	rm -f upload
