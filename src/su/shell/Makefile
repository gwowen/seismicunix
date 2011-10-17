# Makefile for ...su/shell

include $(CWPROOT)/src/Makefile.config

SHELLS =		\
	$B/lookpar	\
	$B/maxdiff	\
	$B/recip	\
	$B/rmaxdiff	\
	$B/suagc	\
	$B/suband	\
	$B/sudiff	\
	$B/sudoc	\
	$B/suenv	\
	$B/suhelp	\
	$B/sufind	\
	$B/sufind2	\
	$B/sugendocs	\
	$B/sukeyword	\
	$B/suname	\
	$B/unglitch

PROGS = $B/percent
LINKS =			\
	$B/susum	\
	$B/suprod	\
	$B/suquo	\
	$B/suptsum	\
	$B/suptprod	\
	$B/suptquo	\
	$B/suptdiff

INSTALL:	$(SHELLS) $(PROGS) $(LINKS)
	@touch $@

$(SHELLS):	$(SHTARGET)
	@-rm -f $@
	@cp $? $@
	@$(MCHMODLINE)
	@echo $(@F) installed in $B

$(LINKS):	$B/sudiff
	@-rm -f $(LINKS)
	@ln $B/sudiff $B/susum
	@ln $B/sudiff $B/suprod
	@ln $B/sudiff $B/suquo
	@ln $B/sudiff $B/suptdiff
	@ln $B/sudiff $B/suptsum
	@ln $B/sudiff $B/suptprod
	@ln $B/sudiff $B/suptquo
	@echo links installed: $(LINKS)

$(PROGS):	$(CTARGET)
	@$(CC) $? -o $@ $(LFLAGS)
	@$(MCHMODLINE)
	@echo $(@F) installed in $B

list:
	@ls $(SHELLS) | pr -5 -t -l22 | p 

remake:
	@-touch *.sh
	@-rm -f $(PROGS) $(SHELLS) $(LINKS)
	@$(MAKE)


.PRECIOUS	= print
