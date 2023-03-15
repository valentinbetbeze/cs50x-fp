PROGNAME := $(basename $(firstword $(MAKECMDGOALS)))
SRC := $(PROGNAME).c

all: $(PROGNAME)

$(PROGNAME): $(SRC)
	gcc -o $(PROGNAME) $(SRC)

clean:
	rm -f $(PROGNAME)

.PHONY: clean

