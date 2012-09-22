python-rfidtag
==============

Python packaging of tiki/touchatag reading code

This is a basic library for accessing tikitag (and touchatag) tag ids from
within python.

This is my initial packaging of the pytiki code by 
    * Younghun Kim
    * Thomas Schmid
    * Aaron Dunn

(as per : http://groups.google.com/group/tikitag/browse_frm/thread/c3aa678690800240# )
The original license for this code is GPL. This means that any program that
uses this code must also be GPL.  This fundamentally means you may not close
this source, nor may any application that uses this code be closed.  See the
license for exact wording and details.

All the files inside src/ are unmodified. Both the examples in examples/ are
modified slightly - dd.p is renamed to "basic.py", test.py has try...finally
added to ensure the tikitag reader is released cleanly.

Given this wraps a small piece of code using swig, it's plausible that this
could be adapted to support other dynamic languages as well.  If you do
this, please contribute back a patch.

Original code:
    * Younghun Kim
    * Thomas Schmid
    * Aaron Dunn

This packaging (setup.py, directory structure, etc): 
    * Michael Sparks

packaging contact: sparks.m@gmail.com
See also: http://groups.google.com/group/tikitag

http://groups.google.com/group/tikitag/browse_frm/thread/c3aa678690800240#

Usage
=====

Please see the example code to see usage. One caveat - due to using libusb,
your code MUST run as root - in order to have appropriate permissions to
open the tikitag device.

Again, if anyone has a patch or suggestion to avoid running as root, please
share.

---

Michael Sparks, February 2009
sparks.m@gmail.com

Who am I?
    * Michael Sparks - sparks.m@gmail.com . If I'm stepping on anyone's toes
      releasing this packaging, please let me know. (This readme will
      probably be replaced by something nicer, but this is a placemark
      before I release it and as per GPL I'm marking my changes including
      license file etc)

