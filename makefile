CC		= gcc
CFLAGS	= -Wall -O2 -g
LDFLAGS	= -lglfw -lGLU -lGL -lm

BINDIR	= bin/
SRCDIR	= src/
OBJDIR	= obj/

# Fichiers globaux
GLOB_OBJ_DIR= $(OBJDIR)3D_tools.o

# Fichiers base.c
OBJ_base= ex01/base.o
EXEC_base= base.out

# Fichiers corridor
OBJ_CRD= $(GLOB_OBJ_DIR) $(OBJDIR)draw_scene.o $(OBJDIR)corridor/corridor.o
EXEC_CRD= corridor.out

# Fichiers raquette
OBJ_RQT= $(GLOB_OBJ_DIR) $(OBJDIR)draw_scene.o $(OBJDIR)raquette/raquette.o
EXEC_RQT= raquette.out


# Fichiers obstacles
OBJ_OBS= $(GLOB_OBJ_DIR) $(OBJDIR)draw_scene.o $(OBJDIR)obstacles/obstacles.o
EXEC_OBS= obstacles.out

# Fichiers collisions
OBJ_COL= $(OBJ_RQT) $(OBJDIR)obstacles/obstacles.o $(OBJDIR)collisions/collisions.o
EXEC_COL= collisions.out


# Regles compilation projet

all : raquette #corridor #obstacles #collisions

corridor : $(OBJ_CRD)
	$(CC) $(CFLAGS) $(OBJ_CRD) -o $(BINDIR)/$(EXEC_CRD) $(LDFLAGS)

raquette : $(OBJ_RQT)
	$(CC) $(CFLAGS) $(OBJ_RQT) -o $(BINDIR)$(EXEC_RQT) $(LDFLAGS)


obstacles : $(OBJ_OBS)
	$(CC) $(CFLAGS) $(OBJ_OBS) -o $(BINDIR)$(EXEC_OBS) $(LDFLAGS)

collisions : $(OBJ_COL)
	$(CC) $(CFLAGS) $(OBJ_COL) -o $(BINDIR)$(EXEC_COL) $(LDFLAGS)


clean :
	rm -rf *~
	rm -rf $(SRCDIR)*/*~
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)*

$(OBJDIR)%.o: $(SRCDIR)%.c
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
