TOPICS   = $(wildcard topics/*.md)
PDFS     = $(patsubst topics/%.md, pdfs/%.pdf, $(TOPICS))

all: $(PDFS)

pdfs/%.pdf: topics/%.md
	@mkdir -p pdfs
	pandoc $< -o $@ --pdf-engine=xelatex -V geometry:margin=1in

combined.pdf: $(TOPICS)
	pandoc $(TOPICS) -o pdfs/combined.pdf --pdf-engine=xelatex --toc -V geometry:margin=1in

clean:
	rm -f pdfs/*.pdf
