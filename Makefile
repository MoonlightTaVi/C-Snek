# Run from the root directory

CC=gcc

APPNAME=Snek
BUILDDIR=build


all: setup app finish

setup:
	rm -rf ./$(BUILDDIR)
	mkdir ./$(BUILDDIR)

app:
	$(CC) src/main.c -o ./$(BUILDDIR)/$(APPNAME)

finish:
	chmod +x ./$(BUILDDIR)/$(APPNAME)

run:
	./$(BUILDDIR)/$(APPNAME)

clean:
	rm -rf ./$(BUILDDIR)
