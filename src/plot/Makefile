# Generated automatically from Makefile.in by configure.
SHELL = /bin/sh

CC	= gcc
CFLAGS	= -g -O2
PLOTOBJ	= 
OBJDIR	= ../objsLinux_x86_64
INC	= -I../include -I..
DEFS	= -DHAVE_CONFIG_H


all: ${PLOTOBJ}

${OBJDIR}/plot_sl.o: plot_sl.c
	${CC} $(CFLAGS) $(INC) -c ${DEFS} -o $@ plot_sl.c

${OBJDIR}/draw_frame.o: draw_frame.c
	${CC} $(CFLAGS) $(INC) -c ${DEFS} -o $@ draw_frame.c

${OBJDIR}/draw_histo.o: draw_histo.c
	${CC} $(CFLAGS) $(INC) -c ${DEFS} -o $@ draw_histo.c

${OBJDIR}/draw_graph.o: draw_graph.c
	${CC} $(CFLAGS) $(INC) -c ${DEFS} -o $@ draw_graph.c

${OBJDIR}/matrix.o: matrix.c
	${CC} $(CFLAGS) $(INC) -c ${DEFS} -o $@ matrix.c

${OBJDIR}/graph.o: graph.c
	${CC} $(CFLAGS) $(INC) -c ${DEFS} -o $@ graph.c

${OBJDIR}/scale1.o: scale1.c
	${CC} $(CFLAGS) $(INC) -c ${DEFS} -o $@ scale1.c

${OBJDIR}/getinput.o: getinput.c
	${CC} $(CFLAGS) $(INC) -c ${DEFS} -o $@ getinput.c

${OBJDIR}/getrange.o: getrange.c
	${CC} $(CFLAGS) $(INC) -c ${DEFS} -o $@ getrange.c

test:	test.c ${PLOTOBJ} ${OBJDIR}/memory.o
	${CC} $(CFLAGS) $(INC) -g ${PLOTOBJ} ${OBJDIR}/memory.o -o test test.c -lslang -lplot -lm
