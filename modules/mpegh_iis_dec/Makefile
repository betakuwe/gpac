include ../../config.mak

vpath %.c $(SRC_PATH)/modules/mpegh_iis

CFLAGS= $(OPTFLAGS) -I"$(SRC_PATH)/include"

ifeq ($(DEBUGBUILD), yes)
CFLAGS+=-g
LDFLAGS+=-g
endif

ifeq ($(GPROFBUILD), yes)
CFLAGS+=-pg
LDFLAGS+=-pg
endif

SRCS := $(OBJS:.o=.c) 

OBJS=mpegh_iis_dec.o

CFLAGS+=-DGPAC_HAS_MPEGH_IIS
#local faad lib
EXTRALIBS+=-L../../extra_lib/lib/gcc
CFLAGS+= -I"$(LOCAL_INC_PATH)/faad"
endif
EXTRALIBS+= -laacDecoder

LIB=gm_mpegh_iis_dec$(DYN_LIB_SUFFIX)


all: $(LIB)


$(LIB): $(OBJS)
	$(CC) $(SHFLAGS) -o ../../bin/gcc/$@ $(OBJS) -L../../bin/gcc -lgpac $(EXTRALIBS) $(LDFLAGS)
ifeq ($(STATICBUILD),yes)
	$(CC) $(SHFLAGS) -o ../../bin/gcc/gm_mpegh_iis_dec-static$(DYN_LIB_SUFFIX) $(OBJS) -L../../bin/gcc -lgpac_static $(EXTRALIBS) $(LDFLAGS)
endif

clean: 
	rm -f $(OBJS) ../../bin/gcc/$(LIB)

dep: depend

depend:
	rm -f .depend	
	$(CC) -MM $(CFLAGS) $(SRCS) 1>.depend

distclean: clean
	rm -f Makefile.bak .depend

-include .depend
