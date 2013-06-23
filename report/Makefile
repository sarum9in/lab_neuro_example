all: lab1.pdf lab2.pdf

%.pdf: %.tex title.tex Task1NeuralNetwork.ps $(wildcard *.png)
	xelatex $<
	xelatex $<

open.%: lab%.pdf
	xdg-open $<

%.ps: %.svg
	convert $< $@

clean:
	rm -f main.pdf *.aux *.log *.toc

.PHONY: all open1 open2 clean
