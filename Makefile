#----------------------------------------------------------------------------
#       Macros
#----------------------------------------------------------------------------
CXX           = g++
SED           = sed
RM            = rm -rf
MKDIR         = mkdir -p
TESTDIRSTART  = test -d
NUL           = /dev/null
TESTDIREND    = ||

#GENFLAGS      = -g
GENFLAGS      = 
LD            = $(CXX) $(CCFLAGS) $(CPPFLAGS)
#CPPFLAGS      = $(GENFLAGS) -D_REENTRANT -fPIC
CPPFLAGS      = $(GENFLAGS) -D_REENTRANT -fPIC -O2
WORKDIR       = $(shell pwd)
LDLIBS        = -ldl
LDFLAGS       = -L"./obj"
COMPILE.cc    = $(CXX) $(CCFLAGS) $(CPPFLAGS) -c
LINK.cc       = $(LD) 
OUTPUT_OPTION = -o "$@"

DEPENDENCIES  = .depend
SRCDIR        = source
OBJDIR        = obj
SRCS          = $(wildcard $(SRCDIR)/*.cxx)
OBJS          = $(patsubst $(SRCDIR)/%.cxx,$(OBJDIR)/%.o,$(SRCS)) 
MAINSRCDIR    = main
MAINOBJDIR    = obj
MAINSRCS      = $(wildcard $(MAINSRCDIR)/*.cxx)
MAINOBJS      = $(patsubst $(MAINSRCDIR)/%.cxx,$(MAINOBJDIR)/%.o,$(MAINSRCS)) 
BINDIR        = .
BIN           = $(patsubst $(MAINSRCDIR)/%.cxx,$(BINDIR)/%,$(MAINSRCS))

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.PHONY : clean all gendir depend contrib

all: gendir contrib $(BIN) $(DEPENDENCIES)
	$(MAKE) -C processor

contrib:
	$(MAKE) -C crfpp

$(BINDIR)/%: $(MAINOBJDIR)/%.o $(OBJS)
	$(LINK.cc) $^ $(OUTPUT_OPTION) $(LDFLAGS) $(LDLIBS)

gendir: 
	-$(TESTDIRSTART) "$(OBJDIR)" $(TESTDIREND) $(MKDIR) "$(OBJDIR)"

depend: $(DEPENDENCIES)

$(DEPENDENCIES):
	$(CC) -MM $(SRCS) > $(DEPENDENCIES)
	$(SED) -i 's/^\([^ ]\)/$(OBJDIR)\/\1/g' $(DEPENDENCIES)

$(OBJDIR)/%.o: $(SRCDIR)/%.cxx
	$(COMPILE.cc) $(EXPORTFLAGS) $(OUTPUT_OPTION) $<

$(MAINOBJDIR)/%.o: $(MAINSRCDIR)/%.cxx
	$(COMPILE.cc) $(EXPORTFLAGS) $(OUTPUT_OPTION) $<

clean:
	-$(RM) $(OBJS)
	-$(RM) $(MAINOBJS)
	-$(RM) $(BIN)
	-$(RM) $(DEPENDENCIES)
	$(MAKE) -C processor clean
	$(MAKE) -C crfpp clean

-include $(DEPENDENCIES)
