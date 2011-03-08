#****************************************************************************
#
# Makefile for CRF++
#
# This is a GNU make (gmake) makefile
#****************************************************************************

CC     := gcc
CXX    := g++
LD     := g++
AR     := ar cur
RANLIB := ranlib

CFLAGS        := -DHAVE_CONFIG_H -I. -Wall -O2 -fPIC -shared
LIBS		  :=
CXXFLAGS      := ${CFLAGS} 
LDFLAGS       := 
INCS          :=


#****************************************************************************
# Makefile code common to all platforms
#****************************************************************************

CFLAGS   := ${CFLAGS}   ${DEFS}
CXXFLAGS := ${CXXFLAGS} ${DEFS}

#****************************************************************************
# Targets of the build
#****************************************************************************

BINDIR = ../obj
OUTPUT := $(BINDIR)/libcrfpp.a

all: ${OUTPUT}


#****************************************************************************
# Source files
#****************************************************************************

SRCDIR = .
SRCS   = $(wildcard $(SRCDIR)/*.cpp)
OBJDIR = ../obj
OBJS   = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/crfpp_%.o, $(SRCS))

#****************************************************************************
# Output
#****************************************************************************

${OUTPUT} : ${OBJS}
	${AR} $@  ${OBJS}
	${RANLIB} $@

#****************************************************************************
# common rules
#****************************************************************************

# Rules for compiling source files to object files
$(OBJDIR)/crfpp_%.o : $(SRCDIR)/%.cpp
	${CXX} -c ${CXXFLAGS} ${INCS} $< -o $@

dist:
	bash makedistlinux

.PHONY : clean
clean:
	-rm -f ${OBJS} ${OUTPUT}
