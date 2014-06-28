#!/usr/bin/python

import sys
import usb.core
from time import sleep
from threading import Thread,Lock

bmRequestTypeIn = usb.util.build_request_type(usb.util.CTRL_IN, usb.util.CTRL_TYPE_VENDOR, usb.util.CTRL_RECIPIENT_INTERFACE)
bmRequestTypeOut = usb.util.build_request_type(usb.util.CTRL_OUT, usb.util.CTRL_TYPE_VENDOR, usb.util.CTRL_RECIPIENT_INTERFACE)

class Badge:
	def __init__(self, device):
		self.dev = device
		self.lock = Lock()

	def send(self, data):
		if len(data) >= 31:
			data = data[:30]
		self.lock.acquire()
		self.dev.ctrl_transfer(bmRequestTypeOut, 0x61, 0, 0, data)
		self.lock.release()

	def recv(self):
		self.lock.acquire()
		ret = self.dev.ctrl_transfer(bmRequestTypeIn, 0x60, 0, 0, 16)
		self.lock.release()
		raw = ''.join(chr(x) for x in ret)
		if len(raw) > 0:
		    return raw
		else:
		    return None


class OutputMonitor(Thread):
	def __init__(self, badge, manager):
		Thread.__init__(self)
		self.badge = badge
	        self.manager = manager
		self.cancel = False

	def run(self):
		while not self.cancel:
            		try:
    				data = self.badge.recv()
	    			if data is not None:
		    			sys.stdout.write(data)
			    		sys.stdout.flush()
    				sleep(0.01)
            		except Exception,e:
                		self.manager.exit()

class InputMonitor(Thread):
	def __init__(self, badge, manager):
		Thread.__init__(self)
		self.badge = badge
	        self.manager = manager
		self.cancel = False

	def run(self):
		while not self.cancel:
			line = sys.stdin.readline()
        		if line == 'exit\n':
		                self.manager.exit()
                		break
			if len(line) > 0:
				self.badge.send(line)

class Console:
    def run(self):
        # find device
        print '*** Ndh2k14 serial console ***'
        print ' - waiting for device ...'

        device = None
        while device is None:
            device = usb.core.find(idProduct=0x05dc, idVendor=0x16c0)
        print ' - device detected, spawning console'

        self.badge = Badge(device)
        self.outputmon = OutputMonitor(self.badge, self)
        self.inputmon = InputMonitor(self.badge, self)
        self.outputmon.start()
        self.inputmon.start()
	#self.inputmon.join()

    def exit(self):
    	self.inputmon.cancel = True
	self.outputmon.cancel = True

c = Console()
c.run()

