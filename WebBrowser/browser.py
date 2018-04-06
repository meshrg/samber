# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'browser.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui
from PyQt4.QtCore import QTimer
from PyQt4.QtGui import QPixmap,QVBoxLayout,QHBoxLayout

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_MainWindow(object):
	
	
	

    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(990, 720)
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        
        self.ui=QtGui.QLabel(self.centralwidget)
        self.ui.setGeometry(QtCore.QRect(20, 20, 988, 174))
        pixmap = QPixmap( './banner_nuevo_2016.png')
        self.ui.setPixmap(pixmap)

        ##################################################
        self.webView = QtWebKit.QWebView(self.centralwidget)
        self.webView.setGeometry(QtCore.QRect(20, 250, 950, 421))
        self.webView.load(QtCore.QUrl("recorrido.html"))
        self.webView.setObjectName(_fromUtf8("webView"))
        MainWindow.setCentralWidget(self.centralwidget)
        MainWindow.setWindowTitle(_translate("Form", "Posición: Histórico", None))
        self.la=QVBoxLayout()
        self.la.addStretch()
        self.la.addWidget(self.ui)
        self.la.addWidget(self.webView)

        self.centralwidget.setLayout(self.la)

        

	
		
        QtCore.QMetaObject.connectSlotsByName(MainWindow)
		

		
		
		

from PyQt4 import QtWebKit

if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    MainWindow = QtGui.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())

