import pytiki as ti
ti.tiki_init()
ti.tiki_config(5000,0)

#for i in range(100):
while True:
	num = ti.tiki_seek()
	if num == 0:
		print "No tag found"
	elif num == 1:
        	print "%x"%ti.tiki_getID1()
	elif num == 2:
		print "%x"%ti.tiki_getID1()
		print 
		print "%x"%ti.tiki_getID2()
	else:
		print "Nope"

ti.tiki_close()
