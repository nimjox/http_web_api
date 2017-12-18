#=========================================================
# Makefile of project "API Daemon"
#=========================================================

PATH_OBJ    = ./obj/

CXXFLAGS += -std=c++11 -g -Wall

NAME = apid
LIBS = -lm -lpthread

DEPEND_FILES = 

DEPEND_FILES += $(PATH_OBJ)apid.o


.PHONY: objdir 

all: $(NAME)

objdir:
	@[ -d $(PATH_OBJ) ] || mkdir $(PATH_OBJ)

$(NAME): $(DEPEND_FILES) | objdir 
	$(CXX) $(LDFLAGS) -o $(NAME) $(DEPEND_FILES) $(LIBS)

#Rule for building object files .cpp files with xxxxxxx-g++.
$(PATH_OBJ)%.o:%.cpp | objdir 
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c -o $@    $< 
 
clean:
	rm -f $(PATH_OBJ)*.o
	rm -f $(NAME)
