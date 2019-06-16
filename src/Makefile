TARGETNAME := lessdb
CLANG-FORMAT := clang-format-8

build/$(TARGETNAME).o:
	@mkdir -p $(@D)
	@$(CXX) -std=c++11 -c LESSDB.cpp -o "$@"
	@echo Finished building: LESSDB.cpp

format:
	@echo Checking code formatting...
	@find . -regex '.*\.\(cpp\|hpp\|h\|cc\|cxx\)' -exec $(CLANG-FORMAT) -i {} \;

clean:
	@echo Cleaning up.
	@rm -rf build/

#debugging
print-%:
	@echo '$*=$($*)'