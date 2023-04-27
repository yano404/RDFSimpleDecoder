TARGET = rdf2tree
CXX = g++
CXXFLAGS = -g -O2 -Wall $(shell root-config --cflags)
LDFLAGS = $(shell root-config --libs)
SRCDIR = ./src
INCLUDE += -I$(SRCDIR)/decoder
INCLUDE += -I$(SRCDIR)/rdf
OBJDIR = ./obj

# main
OBJ += main.o

# RDF
OBJ += rdf/RDFEventStore.o

# Decoder
OBJ += decoder/ModuleDecoder.o
OBJ += decoder/V7XXRawData.o
OBJ += decoder/ModuleDecoderV7XX.o
OBJ += decoder/ModuleDecoderV560.o

OBJS = $(addprefix $(OBJDIR)/, $(OBJ))

$(OBJDIR)/%.o: src/%.cc
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

all: $(OBJS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(INCLUDE) -o $(TARGET) $(OBJS)

clean:
	rm -rf $(OBJS) $(TARGET)
