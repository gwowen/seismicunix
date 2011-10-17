#makefile for vplusz

include $(CWPROOT)/src/Makefile.config

F77  = g77 -c  
LINK = g77  
CC   = cc  -c  

LDLIBS = -lm 

S = .

vpluszf = $S/vpluszf
vpluszc = $S/vpluszc

OBJS = $(vpluszf).o \
       $(vpluszc).o

$B/vpluszf: $(OBJS) 
	$(LINK) $(OBJS) -o $B/vplusz $(LDLIBS) 

$(vpluszf).o: $(vpluszf).f
	$(F77) -I. $(vpluszf).f -o $(vpluszf).o 

$(vpluszc).o: $(vpluszc).c
	cc -c $(vpluszc).c -o $(vpluszc).o

clean:  /bin/rm *.o
