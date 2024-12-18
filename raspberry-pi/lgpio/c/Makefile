CC = gcc
#get all .c files in current directory 
src=$(wildcard ./*.c)
#matches the corresponding files in the current directory
obj=$(patsubst ./%.c,./%.o,$(src))
#link to library
DLIBS=-llgpio -lm
#name of the excutable file
app=main

$(app):$(obj)
	$(CC) $(obj) -o $(app) $(DLIBS)

#output all .o files
$(obj):./%.o:./%.c	
	$(CC) -c $< -o $@ $(DLIBS)

.PHONY:clean all
clean:
	-rm *.o $(app)
$(info clean successful)

#this file should be located in current root directory
#the name of excutable file can modify in "app = main" 
#make clean command is clear all output files 
#if has other library then should be added in DLIBS
