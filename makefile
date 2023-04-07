CC		= gcc
CFLAGS	= -Wall -O2 -g
LDFLAGS	= -lglfw -lGLU -lGL -lm

BINDIR	= bin/
SRCDIR	= src/
OBJDIR	= obj/

# Fichiers base.c
OBJ_base= ex01/base.o
EXEC_base= base.out

# Fichiers corridor
OBJ_CRD= corridor/corridor.o
EXEC_CRD= corridor.out

# Fichiers raquette
OBJ_RQT= raquette/raquette.o
EXEC_RQT= raquette.out

# Regles compilation projet

all : raquette corridor

corridor : $(OBJDIR)$(OBJ_CRD)
	$(CC) $(CFLAGS) $(OBJDIR)$(OBJ_CRD) -o $(BINDIR)$(EXEC_CRD) $(LDFLAGS)

raquette : $(OBJDIR)$(OBJ_RQT)
	$(CC) $(CFLAGS) $(OBJDIR)$(OBJ_RQT) -o $(BINDIR)$(EXEC_RQT) $(LDFLAGS)

clean :
	rm -rf *~
	rm -rf $(SRCDIR)*/*~
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)*

$(OBJDIR)%.o: $(SRCDIR)%.c
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
