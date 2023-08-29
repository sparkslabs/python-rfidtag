#! /usr/bin/python

from rfidtag import *
import Axon
import time

class RFIDTagReader(Axon.ThreadedComponent.threadedcomponent):
    def main(self):
        try:
            rfidtag_init()
            rfidtag_config(5000,0)

            while True:
                num = rfidtag_seek()
                if num == 1:
                    self.send( (time.time(), [ hex( rfidtag_getID1() ) ] , self.name), "outbox")
                elif num == 2:
                    self.send( (time.time(), [ hex( rfidtag_getID1() ) , hex( rfidtag_getID2() ) ] , self.name), "outbox")
                time.sleep(0.01)

        finally:
            rfidtag_close()


from Kamaelia.Chassis.Pipeline import Pipeline
from Kamaelia.Util.Console import ConsoleEchoer

Pipeline( RFIDTagReader(), ConsoleEchoer(use_repr=True) ).run()


