# Generated automatically from Makefile.in by configure.
# Makefile for vamps 0.99i
#
#       Copyright (C) 1995 Jaap Schellekens.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to
# the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

VERSION = 0.99i
DVIPS = dvips
INSTALL = /usr/bin/install -c
MANDIR =  /usr/local/man
MKIDX = makeindex
LATEX = latex
ROFF = nroff -man
BIBTEX = bibtex
BIB = /home/schjpriv/bibtex/puerto-rico  /home/schjpriv/bibtex/artall.bib /home/shjpriv/bibtex/watm

def:
	@echo Targets are:
	@echo man-install -- install manual pages in ${MANDIR}
	@echo doc -- make documentation from LaTeX source
	@echo clean -- clean

man/vamps.1: man/vamps.1.top man/vamps.1.tail
	(cd ../share; rm sldoc.idx; cat *.sl intrin.doc > man/vampsdoc.tmp)
	(cd ../share; vamps -I sldoc.slx -i man/vampsdoc.tmp | sort > sldoc.idx )
	(cd ../share; vamps -I sldoc.slx -t man/vampsdoc.tmp ) > man/vamps.1.int
	(cd ../share; rm man/vampsdoc.tmp sldoc.idx)
	cat man/vamps.1.top man/vamps.1.int man/vamps.1.tail > man/vamps.1

bibfile:
	makebib ${BIB} `cat *.aux | grep \\citation`

intrin.tex: ../share/intrin.doc
	(cd ../share; vamps -I sldoc.slx -l intrin.doc) > intrin.tex

intrinsl.tex: ../share/*.sl
	(cd ../share; vamps -I sldoc.slx -l *.sl) > intrinsl.tex

man-install: man/vamps.1
	sed "s/VERSION/`cat ../0version`/g" < man/vamps.1 > ${MANDIR}/man1/vamps.1
	sed "s/VERSION/`cat ../0version`/g" < man/vsel.1 > ${MANDIR}/man1/vsel.1
	sed "s/VERSION/`cat ../0version`/g" <man/getstr.1 > ${MANDIR}/man1/getstr.1
	sed "s/VERSION/`cat ../0version`/g" < man/issec.1 > ${MANDIR}/man1/issec.1
	sed "s/VERSION/`cat ../0version`/g" < man/vangenu.1 > ${MANDIR}/man1/vangenu.1
	sed "s/VERSION/`cat ../0version`/g" < man/sldoc.1 > ${MANDIR}/man1/sldoc.1
	sed "s/VERSION/`cat ../0version`/g" < man/vamps.5 > ${MANDIR}/man5/vamps.5

catman: dummy
	sed "s/VERSION/`cat ../0version`/g" < man/vamps.1 | ${ROFF} > ./catman/vamps.p1
	sed "s/VERSION/`cat ../0version`/g" < man/vsel.1 | ${ROFF} > ./catman/vsel.p1
	sed "s/VERSION/`cat ../0version`/g" < man/getstr.1 | ${ROFF} > ./catman/getstr.p1
	sed "s/VERSION/`cat ../0version`/g" < man/issec.1 | ${ROFF} > ./catman/issec.p1
	sed "s/VERSION/`cat ../0version`/g" < man/vangenu.1 | ${ROFF} > catman/vangenu.p1
	sed "s/VERSION/`cat ../0version`/g" < man/sldoc.1 | ${ROFF} > catman/sldoc.p1
	sed "s/VERSION/`cat ../0version`/g" < man/vamps.5 | ${ROFF} > catman/vamps.p5

doc: intrin.tex intrinsl.tex
	${LATEX} vamps
	${MKIDX} vamps.idx
	${LATEX} vamps
	${LATEX} vamps
	${LATEX} execsum
	${LATEX} execsum

doc1: intrin.tex intrinsl.tex
	${LATEX} vamps
	${MKIDX} vamps.idx
	${BIBTEX} vamps
	${LATEX} vamps
	${LATEX} vamps
	${LATEX} execsum
	${BIBTEX} execsum
	${LATEX} execsum
	${LATEX} execsum

ps: doc
	${DVIPS} -o vamps.ps vamps	
	${DVIPS} -o execsum.ps execsum	

clean:
	rm -f *~ vamps.toc vamps.log vamps.con vamps.aux vamps.blg\
		vamps.idx vamps.ilg vamps.ind vamps.lof vamps.lop\
		vamps.lot execsum.aux execsum.blg execsum.log userg.aux\
		casestud.aux

dummy:	
