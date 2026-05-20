# Makefile for tests on Linux and macOS.

EXECS    = app-static app-shared
SYSTEM   = $(shell uname -s)
MACOS    = $(if $(subst Darwin,,$(SYSTEM)),,true)
SO       = $(if $(MACOS),dylib,so)
CXXFLAGS = -fPIC

default: $(EXECS)
test: test-static test-shared
app.o libfoo.o: libfoo.h
app-static: app.o libfoo.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@
app-shared: app.o libfoo.$(SO)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@
libfoo.$(SO): libfoo.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -shared -o $@
test-static: app-static
	./app-static
test-shared: app-shared
	LD_LIBRARY_PATH=. ./app-shared
clean:
	rm -rf $(EXECS) *.o *.a *.so *.dylib *.log *.tmp
