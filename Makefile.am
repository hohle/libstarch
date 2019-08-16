SUBDIRS = libaco src test

ACLOCAL_AMFLAGS = -I m4

SCAN_BUILD ?= scan-build80
CLANG_ANALYZER_OUTPUT_DIR = $(top_builddir)/clang
$(CLANG_ANALYZER_OUTPUT_DIR):
	mkdir -p $@
scan-build: $(CLANG_ANALYZER_OUTPUT_DIR)
	CLANG_ANALYZER_OUTPUT=plist-html \
	    $(SCAN_BUILD) \
	        -o "$(CLANG_ANALYZER_OUTPUT_DIR)" \
	        -analyze-headers \
	        $(MAKE) all
	
	if [ -n "$$(find $(CLANG_ANALYZER_OUTPUT_DIR) -name '*.html')" ]; then \
	    echo "Issues found during static analysis." ; \
	    echo "Look in $(CLANG_ANALYZER_OUTPUT_DIR) for more info." ; \
	    exit 1 ; \
	fi
