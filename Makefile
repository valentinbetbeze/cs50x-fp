PROGNAME := $(basename $(firstword $(MAKECMDGOALS)))
SRC := $(PROGNAME).c
UTILS := utils.c
COMMANDS := commands.c
OBJS := $(SRC:.c=.o) $(UTILS:.c=.o) $(COMMANDS:.c=.o)

all: $(PROGNAME)

$(PROGNAME): $(OBJS)
	gcc -o $(PROGNAME) $(OBJS)

%.o: %.c
	gcc -c $< -o $@

clean:
	rm -f $(PROGNAME) $(OBJS)

.PHONY: clean
