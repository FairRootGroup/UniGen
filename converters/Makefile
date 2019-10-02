include $(TOPDIR)/config/Makearch

# Include directories
INCDIR=-I$(TOPDIR)/base/include
CXXFLAGS+=$(INCDIR)

# Libraries
LDFLAGS:=-L$(TOPDIR)/lib -lUniGen $(LDFLAGS)

# Target files
sources=$(wildcard *.cpp)
objects=$(patsubst %.cpp, %.o, $(sources))
binaries=$(patsubst %.o, $(TOPDIR)/bin/%, $(objects))
objects_static=$(patsubst %.cpp, %.o.static, $(sources))
binaries_static=$(patsubst %.o.static, $(TOPDIR)/bin/%.static, $(objects_static))

include $(TOPDIR)/config/Makebin

