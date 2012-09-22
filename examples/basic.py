import pytiki as ti
ti.tiki_init()
ti.tiki_config(5000,0)
num = ti.tiki_seek()
if num == 1:
        print ti.tiki_getID1()
ti.tiki_close()
