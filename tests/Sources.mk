vpath modules/%.cpp ../
vpath modules/%.c ../

SOURCES_COMMON := \
modules/unity/src/unity.c

#common include dirs
INCLUDE_DIRS_COMMON := \
-I"./" \
-I"./unity" \
-I"../modules" \
-I"../src"