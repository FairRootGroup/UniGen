include $(TOPDIR)/config/Makearch

# Target
TARGET=UniGen

# Include directories
INCDIR=-Iinclude
CXXFLAGS+=$(INCDIR)

# Target files
sources=$(wildcard src/*.cxx)
incl=$(patsubst src/%.cxx, include/%.h, $(sources))
objects=$(patsubst src/%.cxx, %.o, $(sources))
objects_static=$(patsubst src/%.cxx, %.o.static, $(sources))

include $(TOPDIR)/config/Makelib

