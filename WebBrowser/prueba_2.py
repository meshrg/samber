#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  prueba_2.py
#  
#  Copyright 2018 Arnulfo Montoya <nura-amd@nuraamd-HP-Notebook>
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#  
import sys
import os
from PyQt4.QtGui import QApplication
from PyQt4.QtGui import QWidget
from PyQt4.QtGui import QPixmap,QVBoxLayout

from PyQt4.QtWebKit import QWebView
from PyQt4.QtCore import QUrl
from PyQt4.QtCore import QTimer
from B_u import *

class Browser(QWidget):
	def __init__(self,parent=None):
		QWidget.__init__(self, parent)
		self.ui=Ui_Form()
		self.ui.setupUi(self)
		pixmap = QPixmap( './banner_nuevo_2016.png')
		print pixmap.width(),pixmap.height()
		self.ui.L_ima.setGeometry(20,10,pixmap.width(),pixmap.height())
		self.ui.L_ima.setPixmap(pixmap)
		self.Web=QWebView()
		self.la=QVBoxLayout()
		self.la.addWidget(self.Web)
		self.ui.W_map.setLayout(self.la)
		self.timer=QTimer(self)
		self.timer.timeout.connect(self._result_available)
		self.timer.start(5000) #milisegundos

		
	def _result_available(self):
		frame = self.Web.page().mainFrame()
		view.Web.reload()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    view = Browser()
    view.Web.load(QUrl("recorrido.html"))
    view.show()
    app.exec_()

