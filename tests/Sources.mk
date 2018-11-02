vpath src/%.cpp ../

SOURCES_dbms := \
tests.cpp \
src/DBMS.cpp \

#make sure all objects are located in build directory
#also make sure objects have .o extension

OBJECTS_dbms := $(addprefix build/,$(SOURCES_dbms))
OBJECTS_dbms := $(OBJECTS_dbms:.c=.o)
OBJECTS_dbms := $(OBJECTS_dbms:.cpp=.o)