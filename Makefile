# project name (generate executable with this name)
TARGET   = Ubissauphte

CC       = gcc
# compiling flags here
CFLAGS   = -Wall -g

LINKER   = gcc
# linking flags here
LFLAGS   = -Wall -lSDL2 -lSDL2_ttf -lSDL2_mixer -lm

# change these to proper directories where each file should be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin


DIRS	 = $(OBJDIR) $(BINDIR) 


SOURCES  := $(shell find $(SRCDIR) -type f -name "*.c")
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
rm       = rm -rf

.PHONY: DIRS
all: $(DIRS) $(BINDIR)/$(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)
$(BINDIR):
	mkdir -p $(BINDIR)



$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(dir $@) 
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS) $(OBJDIR) $(BINDIR)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"



