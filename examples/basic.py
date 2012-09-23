#!/usr/bin/python

from rfidtag import *

rfidtag_init()
rfidtag_config(5000,0)
num = rfidtag_seek()
if num == 1:
    print rfidtag_getID1()

rfidtag_close()
