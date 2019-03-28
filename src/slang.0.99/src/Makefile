# Generated automatically from Makefile.in by configure.
# -*- sh -*-
#---------------------------------------------------------------------------
# ANSI C compiler
#---------------------------------------------------------------------------
CC              = gcc
CFLAGS 		= -g -O -fno-strength-reduce
LDFLAGS		= 
#---------------------------------------------------------------------------
# ELF specific (Linux)
#---------------------------------------------------------------------------
ELF_CC 		= gcc
ELF_CFLAGS	= -O2 -fno-strength-reduce -fPIC
ELF_LINK        = gcc -shared -Wl,-lm,-t,-soname#
#---------------------------------------------------------------------------
# Set these values to ABSOLUTE path names
#---------------------------------------------------------------------------
SRCDIR = /home/jschellekens/Data/src/vampsgpl/src/slang.0.99/src# Location of sources
OBJDIR = /home/jschellekens/Data/src/vampsgpl/src/slang.0.99/src/objs# Location of objects
ELFDIR = /home/jschellekens/Data/src/vampsgpl/src/slang.0.99/src/elfobjs# Location of elf objects

#---------------------------------------------------------------------------
# Set for termcap support
#TCAPLIB = -ltermcap
TCAPLIB = -ltermcap
#---------------------------------------------------------------------------
# Directory where library is going to go when installed
#---------------------------------------------------------------------------
prefix = /usr
exec_prefix = ${prefix}
install_lib_dir = ${exec_prefix}/lib
install_include_dir = ${prefix}/include

#---------------------------------------------------------------------------
# Misc comamnds (safe to leave these untouched)
#---------------------------------------------------------------------------
RANLIB 		= ranlib
INSTALL		= /usr/bin/install -c
INSTALL_DATA	= ${INSTALL} -m 644
MKINSDIR	= ../autoconf/mkinsdir.sh
RM 		= rm -f
RM_R		= rm -rf
AR_CR 		= ar cr
RMDIR 		= rmdir
LN 		= ln -sf
CP 		= cp
MKDIR		= mkdir

#---------------------------------------------------------------------------
# There should be no need to change anything below here.
#---------------------------------------------------------------------------
THIS_LIB		= slang#
OTHERSTUFF 		= 
THIS_LIB_DEFINES	= -DSLANG
ELF_MAJOR_VERSION	= 0#
ELF_MINOR_VERSION	= 99.38#

CONFIG_H		= config.h
ALL_CFLAGS		= $(CFLAGS) -Dunix $(THIS_LIB_DEFINES)
ALL_ELF_CFLAGS		= $(ELF_CFLAGS) -Dunix $(THIS_LIB_DEFINES)
COMPILE_CMD		= $(CC) -c $(ALL_CFLAGS)
ELFCOMPILE_CMD		= $(ELF_CC) -c $(ALL_ELF_CFLAGS)

NORMAL_LIB = lib$(THIS_LIB).a
OBJDIR_NORMAL_LIB = $(OBJDIR)/$(NORMAL_LIB)

ELFLIB = lib$(THIS_LIB).so#
ELFLIB_MAJOR = $(ELFLIB).$(ELF_MAJOR_VERSION)#
ELFLIB_MAJOR_MINOR = $(ELFLIB).$(ELF_MAJOR_VERSION).$(ELF_MINOR_VERSION)#

ELFDIR_ELF_LIB = $(ELFDIR)/$(ELFLIB_MAJOR_MINOR)#

EXECLIBS = -L$(OBJDIR) -lslang -lm $(TCAPLIB)
EXECDEPS = $(OBJDIR_NORMAL_LIB)

OFILES =  sltermin.o sldisply.o slunix.o slutty.o slang.o slarray.o slclass.o slcmd.o slerr.o slfile.o slgetkey.o slkeymap.o slmalloc.o slmath.o slmemchr.o slmemcmp.o slmemcpy.o slmemset.o slmisc.o slparse.o slprepr.o slregexp.o slrline.o slsearch.o slsmg.o slstd.o sltoken.o sltypes.o slxstrng.o slcurses.o slscroll.o slsignal.o slkeypad.o slerrno.o
OBJS =  $(OBJDIR)/sltermin.o $(OBJDIR)/sldisply.o $(OBJDIR)/slunix.o $(OBJDIR)/slutty.o $(OBJDIR)/slang.o $(OBJDIR)/slarray.o $(OBJDIR)/slclass.o $(OBJDIR)/slcmd.o $(OBJDIR)/slerr.o $(OBJDIR)/slfile.o $(OBJDIR)/slgetkey.o $(OBJDIR)/slkeymap.o $(OBJDIR)/slmalloc.o $(OBJDIR)/slmath.o $(OBJDIR)/slmemchr.o $(OBJDIR)/slmemcmp.o $(OBJDIR)/slmemcpy.o $(OBJDIR)/slmemset.o $(OBJDIR)/slmisc.o $(OBJDIR)/slparse.o $(OBJDIR)/slprepr.o $(OBJDIR)/slregexp.o $(OBJDIR)/slrline.o $(OBJDIR)/slsearch.o $(OBJDIR)/slsmg.o $(OBJDIR)/slstd.o $(OBJDIR)/sltoken.o $(OBJDIR)/sltypes.o $(OBJDIR)/slxstrng.o $(OBJDIR)/slcurses.o $(OBJDIR)/slscroll.o $(OBJDIR)/slsignal.o $(OBJDIR)/slkeypad.o $(OBJDIR)/slerrno.o
ELFOBJS =  $(ELFDIR)/sltermin.o $(ELFDIR)/sldisply.o $(ELFDIR)/slunix.o $(ELFDIR)/slutty.o $(ELFDIR)/slang.o $(ELFDIR)/slarray.o $(ELFDIR)/slclass.o $(ELFDIR)/slcmd.o $(ELFDIR)/slerr.o $(ELFDIR)/slfile.o $(ELFDIR)/slgetkey.o $(ELFDIR)/slkeymap.o $(ELFDIR)/slmalloc.o $(ELFDIR)/slmath.o $(ELFDIR)/slmemchr.o $(ELFDIR)/slmemcmp.o $(ELFDIR)/slmemcpy.o $(ELFDIR)/slmemset.o $(ELFDIR)/slmisc.o $(ELFDIR)/slparse.o $(ELFDIR)/slprepr.o $(ELFDIR)/slregexp.o $(ELFDIR)/slrline.o $(ELFDIR)/slsearch.o $(ELFDIR)/slsmg.o $(ELFDIR)/slstd.o $(ELFDIR)/sltoken.o $(ELFDIR)/sltypes.o $(ELFDIR)/slxstrng.o $(ELFDIR)/slcurses.o $(ELFDIR)/slscroll.o $(ELFDIR)/slsignal.o $(ELFDIR)/slkeypad.o $(ELFDIR)/slerrno.o

SHELL = /bin/sh
#---------------------------------------------------------------------------
# Rules
#---------------------------------------------------------------------------
all: $(OBJDIR) $(CONFIG_H) $(OBJDIR_NORMAL_LIB) $(OTHERSTUFF)
elf: $(ELFDIR) $(CONFIG_H) $(ELFDIR_ELF_LIB)

$(OBJDIR_NORMAL_LIB): $(OBJDIR) $(OBJS)
	-$(RM) $(OBJDIR_NORMAL_LIB)
	cd $(OBJDIR); $(AR_CR) $(NORMAL_LIB) $(OFILES)
	$(RANLIB) $(OBJDIR_NORMAL_LIB)
	@echo ""
	@echo $(NORMAL_LIB) created in $(OBJDIR)
# The following was removed below:
#	cd $(ELFDIR); $(RM) $(ELFLIB_MAJOR); $(LN) $(ELFLIB_MAJOR_MINOR) $(ELFLIB_MAJOR)
$(ELFDIR_ELF_LIB): $(ELFDIR) $(ELFOBJS)
	-$(RM) $(ELFDIR_ELF_LIB)
	cd $(ELFDIR); $(ELF_LINK),$(ELFLIB_MAJOR) -o $(ELFLIB_MAJOR_MINOR) $(OFILES)
	cd $(ELFDIR); $(RM) $(ELFLIB); $(LN) $(ELFLIB_MAJOR) $(ELFLIB)
	@echo ""
	@echo $(ELFLIB_MAJOR_MINOR) created in $(ELFDIR).
	@echo The link $(ELFLIB) to $(ELFLIB_MAJOR_MINOR) was also created.
	@echo ""
$(OBJDIR) :
	-$(MKDIR) $(OBJDIR)
$(ELFDIR) :
	-$(MKDIR) $(ELFDIR)
$(CONFIG_H) : sysconf.h
	-$(CP) sysconf.h $(CONFIG_H)

#---------------------------------------------------------------------------
# Intallation rules
#---------------------------------------------------------------------------
install: $(install_lib_dir) $(install_include_dir) $(OBJDIR_NORMAL_LIB)
	@echo installing $(OBJDIR_NORMAL_LIB) in $(install_lib_dir)
	$(INSTALL_DATA) $(OBJDIR_NORMAL_LIB) $(install_lib_dir)
	$(RANLIB) $(install_lib_dir)/$(NORMAL_LIB)
	chmod 644 $(install_lib_dir)/$(NORMAL_LIB)
	@echo installing slang.h and slcurses.h in $(install_include_dir)
	$(INSTALL_DATA) $(SRCDIR)/slang.h $(install_include_dir)
	$(INSTALL_DATA) $(SRCDIR)/slcurses.h $(install_include_dir)
$(install_include_dir):
	$(MKINSDIR) $(install_include_dir)
$(install_lib_dir):
	$(MKINSDIR) $(install_lib_dir)
install-elf: install elf $(install_lib_dir) $(install_include_dir) $(ELFDIR_ELF_LIB)
	$(INSTALL_DATA) $(SRCDIR)/slang.h $(install_include_dir)
	$(INSTALL_DATA) $(SRCDIR)/slcurses.h $(install_include_dir)
	-$(RM) $(install_lib_dir)/$(ELFLIB)
	-$(RM) $(install_lib_dir)/$(ELFLIB_MAJOR)
	@echo installing $(ELFLIB_MAJOR_MINOR) in $(install_lib_dir)
	$(INSTALL_DATA) $(ELFDIR_ELF_LIB) $(install_lib_dir)
	@echo creating symbolic links to $(ELFLIB_MAJOR_MINOR)
	-cd $(install_lib_dir); $(LN) $(ELFLIB_MAJOR_MINOR) $(ELFLIB)
	@echo ""
	@echo $(ELFLIB_MAJOR_MINOR) created in $(install_lib_dir).
	@echo The link $(ELFLIB) to $(ELFLIB_MAJOR_MINOR) was also created.
	@echo ""
install-links:
	-$(RM) $(install_lib_dir)/$(ELFLIB)
	-$(RM) $(install_lib_dir)/$(ELFLIB_MAJOR)
	cd $(install_lib_dir); $(LN) $(ELFLIB_MAJOR_MINOR) $(ELFLIB_MAJOR)
	cd $(install_lib_dir); $(LN) $(ELFLIB_MAJOR) $(ELFLIB)
	@echo
	@echo A link $(ELFLIB_MAJOR) to $(ELFLIB_MAJOR_MINOR) was created.
	@echo A link $(ELFLIB) to $(ELFLIB_MAJOR) was created.
	@echo ""
#---------------------------------------------------------------------------
# Housekeeping
#---------------------------------------------------------------------------
clean:
	-$(RM) $(OBJDIR)/* *~ $(ELFDIR)/*
distclean: clean
	-$(RM_R) $(OBJDIR) $(ELFDIR) Makefile sysconf.h $(CONFIG_H)
#---------------------------------------------------------------------------
# Object Rules : These are created from the configure script (hopefully)
#---------------------------------------------------------------------------

$(OBJDIR)/sltermin.o : $(SRCDIR)/sltermin.c $(DOT_O_DEPS) $(sltermin_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/sltermin.c

$(OBJDIR)/sldisply.o : $(SRCDIR)/sldisply.c $(DOT_O_DEPS) $(sldisply_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/sldisply.c

$(OBJDIR)/slunix.o : $(SRCDIR)/slunix.c $(DOT_O_DEPS) $(slunix_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slunix.c

$(OBJDIR)/slutty.o : $(SRCDIR)/slutty.c $(DOT_O_DEPS) $(slutty_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slutty.c

$(OBJDIR)/slang.o : $(SRCDIR)/slang.c $(DOT_O_DEPS) $(slang_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slang.c

$(OBJDIR)/slarray.o : $(SRCDIR)/slarray.c $(DOT_O_DEPS) $(slarray_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slarray.c

$(OBJDIR)/slclass.o : $(SRCDIR)/slclass.c $(DOT_O_DEPS) $(slclass_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slclass.c

$(OBJDIR)/slcmd.o : $(SRCDIR)/slcmd.c $(DOT_O_DEPS) $(slcmd_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slcmd.c

$(OBJDIR)/slerr.o : $(SRCDIR)/slerr.c $(DOT_O_DEPS) $(slerr_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slerr.c

$(OBJDIR)/slfile.o : $(SRCDIR)/slfile.c $(DOT_O_DEPS) $(slfile_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slfile.c

$(OBJDIR)/slgetkey.o : $(SRCDIR)/slgetkey.c $(DOT_O_DEPS) $(slgetkey_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slgetkey.c

$(OBJDIR)/slkeymap.o : $(SRCDIR)/slkeymap.c $(DOT_O_DEPS) $(slkeymap_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slkeymap.c

$(OBJDIR)/slmalloc.o : $(SRCDIR)/slmalloc.c $(DOT_O_DEPS) $(slmalloc_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slmalloc.c

$(OBJDIR)/slmath.o : $(SRCDIR)/slmath.c $(DOT_O_DEPS) $(slmath_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slmath.c

$(OBJDIR)/slmemchr.o : $(SRCDIR)/slmemchr.c $(DOT_O_DEPS) $(slmemchr_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slmemchr.c

$(OBJDIR)/slmemcmp.o : $(SRCDIR)/slmemcmp.c $(DOT_O_DEPS) $(slmemcmp_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slmemcmp.c

$(OBJDIR)/slmemcpy.o : $(SRCDIR)/slmemcpy.c $(DOT_O_DEPS) $(slmemcpy_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slmemcpy.c

$(OBJDIR)/slmemset.o : $(SRCDIR)/slmemset.c $(DOT_O_DEPS) $(slmemset_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slmemset.c

$(OBJDIR)/slmisc.o : $(SRCDIR)/slmisc.c $(DOT_O_DEPS) $(slmisc_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slmisc.c

$(OBJDIR)/slparse.o : $(SRCDIR)/slparse.c $(DOT_O_DEPS) $(slparse_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slparse.c

$(OBJDIR)/slprepr.o : $(SRCDIR)/slprepr.c $(DOT_O_DEPS) $(slprepr_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slprepr.c

$(OBJDIR)/slregexp.o : $(SRCDIR)/slregexp.c $(DOT_O_DEPS) $(slregexp_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slregexp.c

$(OBJDIR)/slrline.o : $(SRCDIR)/slrline.c $(DOT_O_DEPS) $(slrline_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slrline.c

$(OBJDIR)/slsearch.o : $(SRCDIR)/slsearch.c $(DOT_O_DEPS) $(slsearch_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slsearch.c

$(OBJDIR)/slsmg.o : $(SRCDIR)/slsmg.c $(DOT_O_DEPS) $(slsmg_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slsmg.c

$(OBJDIR)/slstd.o : $(SRCDIR)/slstd.c $(DOT_O_DEPS) $(slstd_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slstd.c

$(OBJDIR)/sltoken.o : $(SRCDIR)/sltoken.c $(DOT_O_DEPS) $(sltoken_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/sltoken.c

$(OBJDIR)/sltypes.o : $(SRCDIR)/sltypes.c $(DOT_O_DEPS) $(sltypes_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/sltypes.c

$(OBJDIR)/slxstrng.o : $(SRCDIR)/slxstrng.c $(DOT_O_DEPS) $(slxstrng_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slxstrng.c

$(OBJDIR)/slcurses.o : $(SRCDIR)/slcurses.c $(DOT_O_DEPS) $(slcurses_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slcurses.c

$(OBJDIR)/slscroll.o : $(SRCDIR)/slscroll.c $(DOT_O_DEPS) $(slscroll_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slscroll.c

$(OBJDIR)/slsignal.o : $(SRCDIR)/slsignal.c $(DOT_O_DEPS) $(slsignal_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slsignal.c

$(OBJDIR)/slkeypad.o : $(SRCDIR)/slkeypad.c $(DOT_O_DEPS) $(slkeypad_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slkeypad.c

$(OBJDIR)/slerrno.o : $(SRCDIR)/slerrno.c $(DOT_O_DEPS) $(slerrno_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(SRCDIR)/slerrno.c

worm : $(OBJDIR)/worm
	@echo worm created in $(OBJDIR)
$(OBJDIR)/worm : $(OBJDIR)/worm.o $(worm_DEPS) $(EXECDEPS)
	$(CC) -o $(OBJDIR)/worm $(LDFLAGS) $(OBJDIR)/worm.o $(worm_LIBS) $(EXECLIBS)
$(OBJDIR)/worm.o : $(SRCDIR)/worm.c $(DOT_O_DEPS) $(worm_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(worm_INC) $(EXECINC) $(SRCDIR)/worm.c

calc : $(OBJDIR)/calc
	@echo calc created in $(OBJDIR)
$(OBJDIR)/calc : $(OBJDIR)/calc.o $(calc_DEPS) $(EXECDEPS)
	$(CC) -o $(OBJDIR)/calc $(LDFLAGS) $(OBJDIR)/calc.o $(calc_LIBS) $(EXECLIBS)
$(OBJDIR)/calc.o : $(SRCDIR)/calc.c $(DOT_O_DEPS) $(calc_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(calc_INC) $(EXECINC) $(SRCDIR)/calc.c

untic : $(OBJDIR)/untic
	@echo untic created in $(OBJDIR)
$(OBJDIR)/untic : $(OBJDIR)/untic.o $(untic_DEPS) $(EXECDEPS)
	$(CC) -o $(OBJDIR)/untic $(LDFLAGS) $(OBJDIR)/untic.o $(untic_LIBS) $(EXECLIBS)
$(OBJDIR)/untic.o : $(SRCDIR)/untic.c $(DOT_O_DEPS) $(untic_O_DEP)
	cd $(OBJDIR); $(COMPILE_CMD) $(untic_INC) $(EXECINC) $(SRCDIR)/untic.c


$(ELFDIR)/sltermin.o : $(SRCDIR)/sltermin.c $(DOT_O_DEPS) $(sltermin_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/sltermin.c

$(ELFDIR)/sldisply.o : $(SRCDIR)/sldisply.c $(DOT_O_DEPS) $(sldisply_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/sldisply.c

$(ELFDIR)/slunix.o : $(SRCDIR)/slunix.c $(DOT_O_DEPS) $(slunix_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slunix.c

$(ELFDIR)/slutty.o : $(SRCDIR)/slutty.c $(DOT_O_DEPS) $(slutty_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slutty.c

$(ELFDIR)/slang.o : $(SRCDIR)/slang.c $(DOT_O_DEPS) $(slang_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slang.c

$(ELFDIR)/slarray.o : $(SRCDIR)/slarray.c $(DOT_O_DEPS) $(slarray_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slarray.c

$(ELFDIR)/slclass.o : $(SRCDIR)/slclass.c $(DOT_O_DEPS) $(slclass_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slclass.c

$(ELFDIR)/slcmd.o : $(SRCDIR)/slcmd.c $(DOT_O_DEPS) $(slcmd_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slcmd.c

$(ELFDIR)/slerr.o : $(SRCDIR)/slerr.c $(DOT_O_DEPS) $(slerr_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slerr.c

$(ELFDIR)/slfile.o : $(SRCDIR)/slfile.c $(DOT_O_DEPS) $(slfile_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slfile.c

$(ELFDIR)/slgetkey.o : $(SRCDIR)/slgetkey.c $(DOT_O_DEPS) $(slgetkey_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slgetkey.c

$(ELFDIR)/slkeymap.o : $(SRCDIR)/slkeymap.c $(DOT_O_DEPS) $(slkeymap_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slkeymap.c

$(ELFDIR)/slmalloc.o : $(SRCDIR)/slmalloc.c $(DOT_O_DEPS) $(slmalloc_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slmalloc.c

$(ELFDIR)/slmath.o : $(SRCDIR)/slmath.c $(DOT_O_DEPS) $(slmath_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slmath.c

$(ELFDIR)/slmemchr.o : $(SRCDIR)/slmemchr.c $(DOT_O_DEPS) $(slmemchr_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slmemchr.c

$(ELFDIR)/slmemcmp.o : $(SRCDIR)/slmemcmp.c $(DOT_O_DEPS) $(slmemcmp_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slmemcmp.c

$(ELFDIR)/slmemcpy.o : $(SRCDIR)/slmemcpy.c $(DOT_O_DEPS) $(slmemcpy_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slmemcpy.c

$(ELFDIR)/slmemset.o : $(SRCDIR)/slmemset.c $(DOT_O_DEPS) $(slmemset_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slmemset.c

$(ELFDIR)/slmisc.o : $(SRCDIR)/slmisc.c $(DOT_O_DEPS) $(slmisc_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slmisc.c

$(ELFDIR)/slparse.o : $(SRCDIR)/slparse.c $(DOT_O_DEPS) $(slparse_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slparse.c

$(ELFDIR)/slprepr.o : $(SRCDIR)/slprepr.c $(DOT_O_DEPS) $(slprepr_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slprepr.c

$(ELFDIR)/slregexp.o : $(SRCDIR)/slregexp.c $(DOT_O_DEPS) $(slregexp_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slregexp.c

$(ELFDIR)/slrline.o : $(SRCDIR)/slrline.c $(DOT_O_DEPS) $(slrline_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slrline.c

$(ELFDIR)/slsearch.o : $(SRCDIR)/slsearch.c $(DOT_O_DEPS) $(slsearch_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slsearch.c

$(ELFDIR)/slsmg.o : $(SRCDIR)/slsmg.c $(DOT_O_DEPS) $(slsmg_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slsmg.c

$(ELFDIR)/slstd.o : $(SRCDIR)/slstd.c $(DOT_O_DEPS) $(slstd_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slstd.c

$(ELFDIR)/sltoken.o : $(SRCDIR)/sltoken.c $(DOT_O_DEPS) $(sltoken_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/sltoken.c

$(ELFDIR)/sltypes.o : $(SRCDIR)/sltypes.c $(DOT_O_DEPS) $(sltypes_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/sltypes.c

$(ELFDIR)/slxstrng.o : $(SRCDIR)/slxstrng.c $(DOT_O_DEPS) $(slxstrng_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slxstrng.c

$(ELFDIR)/slcurses.o : $(SRCDIR)/slcurses.c $(DOT_O_DEPS) $(slcurses_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slcurses.c

$(ELFDIR)/slscroll.o : $(SRCDIR)/slscroll.c $(DOT_O_DEPS) $(slscroll_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slscroll.c

$(ELFDIR)/slsignal.o : $(SRCDIR)/slsignal.c $(DOT_O_DEPS) $(slsignal_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slsignal.c

$(ELFDIR)/slkeypad.o : $(SRCDIR)/slkeypad.c $(DOT_O_DEPS) $(slkeypad_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slkeypad.c

$(ELFDIR)/slerrno.o : $(SRCDIR)/slerrno.c $(DOT_O_DEPS) $(slerrno_O_DEP)
	cd $(ELFDIR); $(ELFCOMPILE_CMD) $(SRCDIR)/slerrno.c

