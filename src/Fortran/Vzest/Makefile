#makefile for vzest

include $(CWPROOT)/src/Makefile.config

F77  = g77 -c  
LINK = g77  
CC   = cc  -c  
#
LDLIBS =  -lm 
#
S = .

# Please do not leave space at the end
vzestf	        = $S/vzestf
vzestsubf       = $S/vzestsubf
vzestc	        = $S/vzestc

OBJS =  $(vzestf).o \
        $(vzestsubf).o \
        $(vzestc).o 

$B/vzest: $(OBJS) 
	$(LINK) $(OBJS) -o $B/vzest $(LDLIBS) 

$(vzestf).o: $(vzestf).f
	$(F77) -I. $(vzestf).f -o $(vzestf).o 

$(vzestsubf).o: $(vzestsubf).f
	$(F77) -I. $(vzestsubf).f -o $(vzestsubf).o 

$(vzestc).o: $(vzestc).c
	$(CC) -I. $(vzestc).c -o $(vzestc).o 

