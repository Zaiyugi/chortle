#!/usr/bin/env python

from PySide import QtCore, QtGui
from ControlsWidget import *
from GLViewer import *

import math
import numpy
import colorsys

class ChortleWindow(QtGui.QMainWindow):

   OGL_PRESS = 0
   OGL_RELEASE = 1
   OGL_MOUSE_BUTTON_NONE = 0
   OGL_MOUSE_BUTTON_LEFT = 1
   OGL_MOUSE_BUTTON_RIGHT = 2
   OGL_MOUSE_BUTTON_MIDDLE = 3

   def __init__(self):
      super(ChortleWindow, self).__init__()

      self.printer = QtGui.QPrinter()
      self.scaleFactor = 0.0

      self.glviewer = GLViewer(1280, 720)

      self.glviewer.setSizePolicy(QtGui.QSizePolicy.Ignored,
            QtGui.QSizePolicy.Ignored)
      self.glviewer.setStyleSheet("background-color:black")

      self.setCentralWidget(self.glviewer)

      self.createActions()
      self.createMenus()

      self.controls = ControlsWidget()

      self.controls.raymarch_iters.valueChanged.connect(self.updateRaymarchIterations)
      self.controls.fractal_iters.valueChanged.connect(self.updateFractalIterations)
      self.controls.fractal_scale.valueChanged.connect(self.updateFractalScale)
      self.controls.flimit_x.valueChanged.connect(self.updateFLimitX)
      self.controls.flimit_y.valueChanged.connect(self.updateFLimitY)
      self.controls.flimit_z.valueChanged.connect(self.updateFLimitZ)

      self.controls.julia_C_x.valueChanged.connect(self.updateJuliaCx)
      self.controls.julia_C_y.valueChanged.connect(self.updateJuliaCy)
      self.controls.julia_C_z.valueChanged.connect(self.updateJuliaCz)
      
      self.controls.julia_offset_x.valueChanged.connect(self.updateJuliaOffsetX)
      self.controls.julia_offset_y.valueChanged.connect(self.updateJuliaOffsetY)
      self.controls.julia_offset_z.valueChanged.connect(self.updateJuliaOffsetZ)

      self.controls.de_offset.valueChanged.connect(self.updateDEOffset)
      
      self.controls.minRadius2.valueChanged.connect(self.updateMR2)
      self.controls.fixedRadius2.valueChanged.connect(self.updateFR2)
      self.controls.glow_strength.valueChanged.connect(self.updateGlowStrength)
      self.controls.epsilon.valueChanged.connect(self.updateEpsilon)

      self.setWindowTitle("Chortle")
      self.resize(1920, 1080)

   ### Slots ###

   @QtCore.Slot(int)
   def updateRaymarchIterations(self, val):
      self.glviewer.setRaymarchIterations(val)
      # print("updateRaymarchIterations")

   @QtCore.Slot(int)
   def updateFractalIterations(self, val):
      self.glviewer.setFractalIterations(val)
      # print("updateFractalIterations")

   @QtCore.Slot(float)
   def updateFractalScale(self, val):
      self.glviewer.setFractalScale(val)
      # print("updateFractalScale")

   @QtCore.Slot(float)
   def updateFLimitX(self, val):
      self.glviewer.setFLimitX(val)
      # print("updateFLimitX")

   @QtCore.Slot(float)
   def updateFLimitY(self, val):
      self.glviewer.setFLimitY(val)
      # print("updateFLimitY")

   @QtCore.Slot(float)
   def updateFLimitZ(self, val):
      self.glviewer.setFLimitZ(val)
      # print("updateFLimitZ")

   @QtCore.Slot(float)
   def updateJuliaCx(self, val):
      self.glviewer.setJuliaCx(val)

   @QtCore.Slot(float)
   def updateJuliaCy(self, val):
      self.glviewer.setJuliaCy(val)

   @QtCore.Slot(float)
   def updateJuliaCz(self, val):
      self.glviewer.setJuliaCz(val)

   @QtCore.Slot(float)
   def updateJuliaOffsetX(self, val):
      self.glviewer.setJuliaOffsetX(val)

   @QtCore.Slot(float)
   def updateJuliaOffsetY(self, val):
      self.glviewer.setJuliaOffsetY(val)

   @QtCore.Slot(float)
   def updateJuliaOffsetZ(self, val):
      self.glviewer.setJuliaOffsetZ(val)

   @QtCore.Slot(float)
   def updateDEOffset(self, val):
      self.glviewer.setDEOffset(val)

   @QtCore.Slot(float)
   def updateMR2(self, val):
      self.glviewer.setMR2(val)
      # print("updateMR2")

   @QtCore.Slot(float)
   def updateFR2(self, val):
      self.glviewer.setFR2(val)
      # print("updateFR2")

   @QtCore.Slot(float)
   def updateGlowStrength(self, val):
      self.glviewer.setGlowStrength(val)
      # print("updateGlowStrength")

   @QtCore.Slot(float)
   def updateEpsilon(self, val):
      self.glviewer.setEpsilon(val)
      # print("updateEpsilon")

   ### Event Handlers ###

   def mousePressEvent(self, QMouseEvent):
      button = self.OGL_MOUSE_BUTTON_NONE
      if QMouseEvent.button() == QtCore.Qt.LeftButton:
         button = self.OGL_MOUSE_BUTTON_LEFT
      elif QMouseEvent.button() == QtCore.Qt.RightButton:
         button = self.OGL_MOUSE_BUTTON_RIGHT
      elif QMouseEvent.button() == QtCore.Qt.MiddleButton:
         button = self.OGL_MOUSE_BUTTON_MIDDLE

      self.glviewer.handleMouseButtonEvent(button, self.OGL_PRESS, 0, QMouseEvent.x(), QMouseEvent.y())

   def mouseReleaseEvent(self, QMouseEvent):
      button = self.OGL_MOUSE_BUTTON_NONE
      if QMouseEvent.button() == QtCore.Qt.LeftButton:
         button = self.OGL_MOUSE_BUTTON_LEFT
      elif QMouseEvent.button() == QtCore.Qt.RightButton:
         button = self.OGL_MOUSE_BUTTON_RIGHT
      elif QMouseEvent.button() == QtCore.Qt.MiddleButton:
         button = self.OGL_MOUSE_BUTTON_MIDDLE

      self.glviewer.handleMouseButtonEvent(button, self.OGL_RELEASE, 0, QMouseEvent.x(), QMouseEvent.y())

   def mouseMoveEvent(self, QMouseEvent):
      self.glviewer.handleMouseMoveEvent(QMouseEvent.x(), QMouseEvent.y())

   ### UI ###

   def showControls(self):
      self.controls.show()

   def getConfiguration(self):
      self.glviewer.getConfiguration()

   def about(self):
      pass

   def createActions(self):
      self.exit_action = QtGui.QAction("E&xit", self, shortcut="Ctrl+Q",
            triggered=self.close)

      self.show_controls_action = QtGui.QAction("&Controls", self,
            triggered=self.showControls)

      self.get_configuration_action = QtGui.QAction("&Get Config", self,
            triggered=self.getConfiguration)

      self.about_action = QtGui.QAction("&About", self, triggered=self.about)

      self.about_Qt_action = QtGui.QAction("About &Qt", self,
            triggered=QtGui.qApp.aboutQt)

   def createMenus(self):
      self.fileMenu = QtGui.QMenu("&File", self)
      self.fileMenu.addAction(self.show_controls_action)
      self.fileMenu.addSeparator()
      self.fileMenu.addAction(self.exit_action)

      self.viewMenu = QtGui.QMenu("&View", self)
      self.viewMenu.addAction(self.show_controls_action)
      self.viewMenu.addAction(self.get_configuration_action)

      self.helpMenu = QtGui.QMenu("&Help", self)
      self.helpMenu.addAction(self.about_action)
      self.helpMenu.addAction(self.about_Qt_action)

      self.menuBar().addMenu(self.fileMenu)
      self.menuBar().addMenu(self.viewMenu)

   def updateActions(self):
      self.zoom_in_action.setEnabled(not self.fit_to_window_action.isChecked())
      self.zoom_out_action.setEnabled(not self.fit_to_window_action.isChecked())
      self.normal_size_action.setEnabled(not self.fit_to_window_action.isChecked())

   def scaleImage(self, factor):
      self.scaleFactor *= factor

      self.zoom_in_action.setEnabled(self.scaleFactor < 3.0)
      self.zoom_out_action.setEnabled(self.scaleFactor > 0.333)

   def adjustScrollBar(self, scrollBar, factor):
      scrollBar.setValue(int(factor * scrollBar.value()
                        + ((factor - 1) * scrollBar.pageStep()/2)))


if __name__ == '__main__':

   import sys

   app = QtGui.QApplication(sys.argv)
   viewer = ChortleWindow()
   viewer.show()
   sys.exit(app.exec_())
