#!/usr/bin/python

from rfidtag import *

rfidtag_init()
rfidtag_config(5000,0)

while True:
    num = rfidtag_seek()
    if num == 0:
        print "No tag found"
    elif num == 1:
        print "Tag: ", rfidtag_getID1()
    elif num == 2:
        print "Tag 1/2", rfidtag_getID1()
        print 
        print "Tag 2/2", rfidtag_getID2()
    else:
        print "Nope", num

rfidtag_close()
