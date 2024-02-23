PROG = dbylib

ifeq ($(LDFLAGS),"-m32")
LIB = /usr/local/lib/dbylib.a
else
LIB = /usr/local/lib64/dbylib.a
endif

DEF = /usr/local/include

FILES = \
	$(LIB)(dbyCommitFuncs.o)	\
	$(LIB)(dbyClose.o)	\
	$(LIB)(dbyConnect.o)	\
	$(LIB)(dbyDelete.o)	\
	$(LIB)(dbyFreeQuery.o)	\
	$(LIB)(dbyInit.o)	\
	$(LIB)(dbyInsert.o)	\
	$(LIB)(dbySelect.o)	\
	$(LIB)(dbySelectCB.o)	\
	$(LIB)(dbySelectCount.o)	\
	$(LIB)(dbySelectDate.o)	\
	$(LIB)(dbySelectMinMaxAvg.o)	\
	$(LIB)(dbySelectAvg.o)	\
	$(LIB)(dbySelectSumDouble.o)	\
	$(LIB)(dbySelectSumLong.o)	\
	$(LIB)(dbyUpdate.o)	\
	$(LIB)(dbyWildCards.o)	\
	$(LIB)(dbyFuncs.o)

.SILENT:

$(LIB): $(FILES)
	make include

$(FILES): dbylib.h

include:
	rm -f $(DEF)/dbylib.h
	cp -pv dbylib.h $(DEF)/dbylib.h

strip:
	ls -l $(LIB)
	strip --strip-unneeded $(LIB)
	ls -l $(LIB)

clean:
	rm -f $(LIB)

all:
	make clean
	make
	make include

.PRECIOUS: $(LIB)

