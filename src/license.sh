#! /bin/sh
# mailhome.sh - send a message back to CWP 
# John Stockwell, Center for Wave Phenomena, 1 August 1997
#set -x

# these items identify the date/release of the codes
DATE="15 Feb 2010"
RELEASE="42"

echo
echo
echo "################################################################"
echo "####### Legal Statement for ${DATE} Release ${RELEASE} of CWP/SU #######"
echo "################################################################"
echo
echo "hit return key to continue"  | tr -d "\012"
read RESP 
echo
	more ./LEGAL_STATEMENT
echo
echo "By answering you agree to abide by the terms and conditions of"
echo "the above LEGAL STATEMENT ?[y/n]"  | tr -d "\012"
read RESP

case $RESP in
	y*|Y*) # continue
	;;
	*) # Stop installation
		exit 1
	;;
		esac

exit 0
