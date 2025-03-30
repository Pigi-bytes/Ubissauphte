TARGET   = Ubissauphte

#------------------------------------------------------------------------------
# Configuration du compilateur
#------------------------------------------------------------------------------
CC       = gcc
CFLAGS   = -Wall -g 
LFLAGS   = -Wall -lSDL2 -lSDL2_ttf -lSDL2_mixer -lm

#------------------------------------------------------------------------------
# Configuration des repertoire
#------------------------------------------------------------------------------
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin
DIRS     = $(OBJDIR) $(BINDIR)

#------------------------------------------------------------------------------
# Couleurs pour les messages
#------------------------------------------------------------------------------
BOLD     = \033[1m
RED      = \033[1;31m
GREEN    = \033[1;32m
YELLOW   = \033[1;33m
BLUE     = \033[1;34m
CYAN     = \033[1;36m
RESET    = \033[0m

#------------------------------------------------------------------------------
# Détection des fichiers
#------------------------------------------------------------------------------
# Recherche récursive des fichiers .c/h
SOURCES  := $(shell find $(SRCDIR) -type f -name "*.c")
INCLUDES := $(shell find $(SRCDIR) -type f -name "*.h")
OBJECTS  := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
RM       = rm -rf
#------------------------------------------------------------------------------
# Règles 
#------------------------------------------------------------------------------
.PHONY: all clean rebuild

all: $(DIRS) $(BINDIR)/$(TARGET)

$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@echo "$(BLUE)Création du répertoire$(RESET) $(BOLD)$(OBJDIR)$(RESET)"

$(BINDIR):
	@mkdir -p $(BINDIR)
	@echo "$(BLUE)Création du répertoire$(RESET) $(BOLD)$(BINDIR)$(RESET)"

$(BINDIR)/$(TARGET): $(OBJECTS)
	@echo "$(YELLOW)Édition de liens...$(RESET)"
	@$(CC) $(OBJECTS) $(LFLAGS) -o $@
	@echo "$(GREEN)Édition de liens terminée !$(RESET) $(CYAN)[$(TARGET)]$(RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compilation de$(RESET) $(BOLD)$<$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)Compilation réussie !$(RESET)"

clean:
	@echo "$(RED)Nettoyage des fichiers générés...$(RESET)"
	@$(RM) $(OBJECTS) $(OBJDIR) $(BINDIR)
	@echo "$(GREEN)Nettoyage terminé !$(RESET)"

rebuild: clean all
	@echo "$(GREEN)Reconstruction complète terminée !$(RESET)\n"