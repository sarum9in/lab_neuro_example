all: lab1.pdf lab2.pdf

%.pdf: %.tex title.tex
	xelatex $<
	xelatex $<

open.%: lab%.pdf
	xdg-open $<

clean:
	rm -f main.pdf *.aux *.log *.toc

.PHONY: all open1 open2 clean
