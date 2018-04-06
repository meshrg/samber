# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'B_ui.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui
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

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName(_fromUtf8("Form"))
        Form.resize(970,590)
        self.W_map = QtGui.QWidget(Form)
        self.W_map.setGeometry(QtCore.QRect(10, 200, 950, 381))
        self.W_map.setObjectName(_fromUtf8("W_map"))
        self.L_ima = QtGui.QLabel(Form)
        self.L_ima.setGeometry(QtCore.QRect(20, 10, 741, 71))
        self.L_ima.setText(_fromUtf8(""))
        self.L_ima.setObjectName(_fromUtf8("L_ima"))
        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(_translate("Form", "Posición tiempo real", None))


class Form(QtGui.QWidget, Ui_Form):
    def __init__(self, parent=None, f=QtCore.Qt.WindowFlags()):
        QtGui.QWidget.__init__(self, parent, f)

        self.setupUi(self)

