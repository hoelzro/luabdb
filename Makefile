TARGETS=$(shell ls makefile.* | sed -e "s/makefile.//")

all:
	@echo "Please choose one of the following targets: $(TARGETS)"

$(TARGETS):
	@make -f makefile.$@

clean:
	rm -f *.o *.so *.dylib
