include $(TOPDIR)/config/Makearch

# Target
TARGET=Tasks

# Include directories
INCDIR=-Iinclude -I$(TOPDIR)/base/include
CXXFLAGS+=$(INCDIR)
rlibmap_deps=-d $(TOPDIR)/lib/libUniGen.so

# Target files
sources=$(wildcard src/*.cxx)
incl=$(patsubst src/%.cxx, include/%.h, $(sources))
objects=$(patsubst src/%.cxx, %.o, $(sources))
objects_static=$(patsubst src/%.cxx, %.o.static, $(sources))

include $(TOPDIR)/config/Makelib

