# Requires the following project directory structure:
#  /bin
#  /obj
#  /src

TARGET     = lab6

CXX        = clang++
CXXFLAGS   = -std=c++11 \
             -Wno-c++98-compat

LD         = clang++ -o
LDFLAGS    = -Wall -pedantic

SRCDIR     = src
INCDIR	   = include
OBJDIR     = obj
BINDIR     = bin

SOURCES   := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES  := $(wildcard $(INCDIR)/*.h)
OBJECTS   := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

RM         = rm -f
MKDIR      = mkdir -p

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LD) $@ $(LDFLAGS) $(OBJECTS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(RM) $(OBJECTS)
	@echo "Cleanup compete!"

.PHONY: remove
remove: clean
	@$(RM) $(BINDIR)/$(TARGET)

.PHONY: dirs
dirs:
	@$(MKDIR) $(SRCDIR) $(INCDIR) $(OBJDIR) $(BINDIR)
	
