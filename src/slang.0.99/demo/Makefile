# Generated automatically from Makefile.in by configure.
# -*- sh -*-

#---------------------------------------------------------------------------
# 1. Choose a C compiler.  It must understand prototypes.
#-----------------------------------------------------------------------------
CC = gcc
CFLAGS = -g -O -fno-strength-reduce
#-----------------------------------------------------------------------------
# 3. Location where object files are placed (Absolute path)
#-----------------------------------------------------------------------------
OBJDIR = /home/jschellekens/Data/src/vampsgpl/src/slang.0.99/demo/objs
SRCDIR = /home/jschellekens/Data/src/vampsgpl/src/slang.0.99/demo
#-----------------------------------------------------------------------------
# 2. Directory where the various libraries are located.
#-----------------------------------------------------------------------------
SLANG_INCLUDE = $(SRCDIR)/../src#             location of slang.h
SLANG_LIB = $(SLANG_INCLUDE)/$(ARCH)objs#     location of libslang.a

#---------------------------------------------------------------------------
# 3. Other libraries
#---------------------------------------------------------------------------
TCAPLIB = -ltermcap
#TCAPLIB = -ltermcap

#----------------------------------------------------------------------------
# End of user configuration
#----------------------------------------------------------------------------

SHELL = /bin/sh
OTHERSTUFF = array simple useropen complex pager keypad smgtest
CONFIG_H = config.h

ALL_CFLAGS = $(CFLAGS) -Dunix -I$(SLANG_INCLUDE)

COMPILE_CMD = $(CC) -c $(ALL_CFLAGS)
EXECLIBS = -L$(SLANG_LIB) -lslang -lm $(TCAPLIB)
EXECDEPS = $(SLANG_LIB)/libslang.a
DOT_O_DEPS = demolib.c

#---------------------------------------------------------------------------
all: $(OBJDIR) $(CONFIG_H) $(OTHERSTUFF)

$(CONFIG_H) : $(SLANG_LIB)/libslang.a ../src/config.h
	/bin/cp ../src/config.h $(CONFIG_H)
$(SLANG_LIB)/libslang.a :
	cd ../src; $(MAKE)
#
$(OBJDIR):
	@mkdir $(OBJDIR)
#---------------------------------------------------------------------------
# Housekeeping
#---------------------------------------------------------------------------
clean:
	-/bin/rm -f $(OBJDIR)/* *~
distclean: clean
	-/bin/rm -rf $(OBJDIR) Makefile
#---------------------------------------------------------------------------
# Everything else from configure script
#---------------------------------------------------------------------------

array : $(OBJDIR)/array
	@echo array created in $(OBJDIR)
$(OBJDIR)/array : $(OBJDIR)/array.o $(array_DEPS) $(EXECDEPS)
	$(CC) -o $(OBJDIR)/array $(LDFLAGS) $(OBJDIR)/array.o $(array_LIBS) $(EXECLIBS)
$(OBJDIR)/array.o : $(SRCDIR)/array.c $(DOT_O_DEPS) $(array_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(array_INC) $(EXECINC) $(SRCDIR)/array.c

complex : $(OBJDIR)/complex
	@echo complex created in $(OBJDIR)
$(OBJDIR)/complex : $(OBJDIR)/complex.o $(complex_DEPS) $(EXECDEPS)
	$(CC) -o $(OBJDIR)/complex $(LDFLAGS) $(OBJDIR)/complex.o $(complex_LIBS) $(EXECLIBS)
$(OBJDIR)/complex.o : $(SRCDIR)/complex.c $(DOT_O_DEPS) $(complex_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(complex_INC) $(EXECINC) $(SRCDIR)/complex.c

simple : $(OBJDIR)/simple
	@echo simple created in $(OBJDIR)
$(OBJDIR)/simple : $(OBJDIR)/simple.o $(simple_DEPS) $(EXECDEPS)
	$(CC) -o $(OBJDIR)/simple $(LDFLAGS) $(OBJDIR)/simple.o $(simple_LIBS) $(EXECLIBS)
$(OBJDIR)/simple.o : $(SRCDIR)/simple.c $(DOT_O_DEPS) $(simple_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(simple_INC) $(EXECINC) $(SRCDIR)/simple.c

useropen : $(OBJDIR)/useropen
	@echo useropen created in $(OBJDIR)
$(OBJDIR)/useropen : $(OBJDIR)/useropen.o $(useropen_DEPS) $(EXECDEPS)
	$(CC) -o $(OBJDIR)/useropen $(LDFLAGS) $(OBJDIR)/useropen.o $(useropen_LIBS) $(EXECLIBS)
$(OBJDIR)/useropen.o : $(SRCDIR)/useropen.c $(DOT_O_DEPS) $(useropen_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(useropen_INC) $(EXECINC) $(SRCDIR)/useropen.c

pager : $(OBJDIR)/pager
	@echo pager created in $(OBJDIR)
$(OBJDIR)/pager : $(OBJDIR)/pager.o $(pager_DEPS) $(EXECDEPS)
	$(CC) -o $(OBJDIR)/pager $(LDFLAGS) $(OBJDIR)/pager.o $(pager_LIBS) $(EXECLIBS)
$(OBJDIR)/pager.o : $(SRCDIR)/pager.c $(DOT_O_DEPS) $(pager_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(pager_INC) $(EXECINC) $(SRCDIR)/pager.c

keypad : $(OBJDIR)/keypad
	@echo keypad created in $(OBJDIR)
$(OBJDIR)/keypad : $(OBJDIR)/keypad.o $(keypad_DEPS) $(EXECDEPS)
	$(CC) -o $(OBJDIR)/keypad $(LDFLAGS) $(OBJDIR)/keypad.o $(keypad_LIBS) $(EXECLIBS)
$(OBJDIR)/keypad.o : $(SRCDIR)/keypad.c $(DOT_O_DEPS) $(keypad_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(keypad_INC) $(EXECINC) $(SRCDIR)/keypad.c

smgtest : $(OBJDIR)/smgtest
	@echo smgtest created in $(OBJDIR)
$(OBJDIR)/smgtest : $(OBJDIR)/smgtest.o $(smgtest_DEPS) $(EXECDEPS)
	$(CC) -o $(OBJDIR)/smgtest $(LDFLAGS) $(OBJDIR)/smgtest.o $(smgtest_LIBS) $(EXECLIBS)
$(OBJDIR)/smgtest.o : $(SRCDIR)/smgtest.c $(DOT_O_DEPS) $(smgtest_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(smgtest_INC) $(EXECINC) $(SRCDIR)/smgtest.c

