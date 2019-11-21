vpath src/%.cpp ../

SOURCES_$(shell basename $(dir $(lastword $(MAKEFILE_LIST)))) := \
src/LESSDB.cpp