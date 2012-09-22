#! /usr/bin/python

import pytiki as ti
import Axon
import time

class TikiTagDownUp(Axon.ThreadedComponent.threadedcomponent):
    def main(self):
        print "Woo"
        tagstatus = {}
        seentag = {}
        try:
            ti.tiki_init()
            ti.tiki_config(196,0)
            while 1:
                num = ti.tiki_seek()
                if num == 0:
                    for i in tagstatus:
#                        self.send(("TAGUP", i), "outbox")
                        print time.time(), "TAG UP  : ", i
                    tagstatus = {}
                elif num == 1:
                    raw_tag_id = ti.tiki_getID1()
                    if seentag.get(raw_tag_id, False):
                        if tagstatus.get(raw_tag_id, "up") != "down":
                            print time.time(), "TAG DOWN: ", raw_tag_id
#                            self.send(("TAGDOWN", raw_tag_id), "outbox")
#                            print time.time(), "TAG DOWN: ", raw_tag_id
                            tagstatus[raw_tag_id] = "down"
                    else:
#                        self.send(("TAGDOWN", raw_tag_id), "outbox")
                        print time.time(), "TAG DOWN:", raw_tag_id
                        seentag[raw_tag_id] = True
                        tagstatus[raw_tag_id] = "down"

        finally:
            ti.tiki_close()


from Kamaelia.Chassis.Pipeline import Pipeline
from Kamaelia.Util.Console import ConsoleEchoer

Pipeline( TikiTagDownUp(), ConsoleEchoer(use_repr=True) ).run()


