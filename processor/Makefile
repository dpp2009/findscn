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
LDLIBS        = -ldl -lrt
LDFLAGS       = 
COMPILE.cc    = $(CXX) $(CCFLAGS) $(CPPFLAGS) -c -shared 
LINK.cc       = $(LD) $(LDFLAGS) $(LDLIBS) 

BTARGETDIR    = ..
SRCDIR        = .
OBJDIR        = ../obj
SEDOBJDIR     = ..\/obj
BIN           = $(BTARGETDIR)/prepare.so $(BTARGETDIR)/crf_ner_nr.so 
DEPENDENCIES  = .depend
OUTPUT_OPTION = -o "$@"
SRCS          = $(wildcard $(SRCDIR)/*.cxx)
OBJS          = $(patsubst $(SRCDIR)/%.cxx,$(OBJDIR)/%.o,$(SRCS)) 

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.PHONY : all
all: gendir $(BIN) $(DEPENDENCIES)

$(BIN): $(OBJS)
	$(LD) $(LDFLAGS) -shared -o $(BTARGETDIR)/crf_ner_nr.so $(OBJDIR)/crf_ner_nr_processor.o $(OBJDIR)/processor.o $(OBJDIR)/libcrfpp.a
	$(LD) $(LDFLAGS) -shared -o $(BTARGETDIR)/prepare.so $(OBJDIR)/prepare_processor.o $(OBJDIR)/processor.o $(OBJDIR)/utf8.o

.PHONY : gendir
gendir: 
	@$(TESTDIRSTART) "$(BTARGETDIR)" $(TESTDIREND) $(MKDIR) "$(BTARGETDIR)"
	@$(TESTDIRSTART) "$(OBJDIR)" $(TESTDIREND) $(MKDIR) "$(OBJDIR)"

.PHONY : depend
depend: $(DEPENDENCIES)

$(DEPENDENCIES):
	$(CC) -MM $(SRCS) > $(DEPENDENCIES)
	$(SED) -i 's/^\([^ ]\)/$(SEDOBJDIR)\/\1/g' $(DEPENDENCIES)

$(OBJDIR)/%.o: $(SRCDIR)/%.cxx
	$(COMPILE.cc) $(EXPORTFLAGS) $(OUTPUT_OPTION) $<

.PHONY : clean
clean:
	-$(RM) $(OBJS)
	-$(RM) $(BIN)
	-$(RM) $(DEPENDENCIES)

-include $(DEPENDENCIES)
