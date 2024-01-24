# Makefile for LaTeX using latexmk

# Variables
LATEXMK = latexmk
MAIN_TEX = main.tex

.PHONY: all clean

all:
	$(LATEXMK) -pdf $(MAIN_TEX)

clean:
	$(LATEXMK) -c
