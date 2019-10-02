# Makefile for the United Generators package
# Created 13-02-2006 by D.Kresan
# e-mail: D.Kresan@gsi.de

# Where we are now
export TOPDIR=$(PWD)

# Default installation path
ifeq ($(strip $(UNIGEN)),)
  export DESTDIR=/usr/local/UniGen
else
  export DESTDIR=$(UNIGEN)
endif

.PHONY : all base genesis static base_static genesis_static clean_all clean clean_genesis install

SUBDIR=base converters input tasks

all :
	@for subdir in $(SUBDIR) ; do \
	  $(MAKE) -C $$subdir all ; \
	done

base :
	$(MAKE) -C base all

genesis : base
	@$(MAKE) -C converters/genesis all
	@$(MAKE) -C tasks/genesis all

static :
	@for subdir in $(SUBDIR) ; do \
	  $(MAKE) -C $$subdir static ; \
	done

base_static :
	$(MAKE) -C base static

genesis_static : base_static
	@$(MAKE) -C converters/genesis static
	@$(MAKE) -C tasks/genesis static

clean_all : clean clean_genesis

clean :
	@for subdir in $(SUBDIR) ; do \
	  $(MAKE) -C $$subdir clean ; \
	done

clean_genesis :
	@$(MAKE) -C converters/genesis clean
	@$(MAKE) -C tasks/genesis clean

install_all : install install_genesis

install :
	@for subdir in $(SUBDIR) ; do \
	  $(MAKE) -C $$subdir install ; \
	done

install_genesis :
	@$(MAKE) -C converters/genesis install
	@$(MAKE) -C tasks/genesis install

htmldoc : all
	$(MAKE) -C base htmldoc
	$(MAKE) -C tasks htmldoc
