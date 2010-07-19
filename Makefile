TARGETS=$(shell ls makefile.* | sed -e "s/makefile.//")

all:
	@echo "Please choose one of the following targets: $(TARGETS)"

linux:
	@make -f makefile.linux

macosx:
	@make -f makefile.macosx

clean:
	rm -f *.o *.so
