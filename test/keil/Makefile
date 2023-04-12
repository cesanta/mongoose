# Do NOT use parenthesis on Windows project dir names
PROJECTS = $(wildcard ../../examples/stm32/nucleo-*-keil-*)

all: $(PROJECTS)
	
# Need to test inside .bat to check and transfer errors
$(PROJECTS): FORCE
	unitest.bat $(subst /,\,$@)

FORCE:
