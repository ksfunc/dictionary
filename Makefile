CC      = gcc
CFLAGS  = -c -pedantic -std=c11 -lpthread `pkg-config --cflags --libs gtk+-3.0`
LDFLAGS = `pkg-config --cflags --libs gtk+-3.0`
LIBS    =
INCLUDE = -I./include
TARGET  = ./bin/$(shell basename `readlink -f .`)
SRCDIR  = ./src
OBJDIR  = ./obj
CSRCS   = $(wildcard $(SRCDIR)/*.c)
OBJS    = $(addprefix $(OBJDIR)/, $(notdir $(CSRCS:.c=.o)))

$(TARGET): $(OBJS) $(LIBS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	-mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	rm $(OBJS) $(TARGET)
