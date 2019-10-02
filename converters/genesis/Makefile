include $(TOPDIR)/config/Makearch

# Include directories
INCDIR=-I$(TOPDIR)/base/include
INCDIR+=-I$(GENESIS)/include
CXXFLAGS+=$(INCDIR)

# Libraries
LIBS=-L$(TOPDIR)/lib -lUniGen
LIBS+=-L$(GENESIS)/lib -lGenesis
LDFLAGS+=$(LIBS)

# Target files
sources=$(wildcard *.cpp)
objects=$(patsubst %.cpp, %.o, $(sources))
binaries=$(patsubst %.o, $(TOPDIR)/bin/%, $(objects))
objects_static=$(patsubst %.cpp, %.o.static, $(sources))
binaries_static=$(patsubst %.o.static, $(TOPDIR)/bin/%.static, $(objects_static))

include $(TOPDIR)/config/Makebin

