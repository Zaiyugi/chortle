#!/usr/bin/env python

from PySide import QtCore, QtGui
from ControlsWidget import *
from GLViewer import *

import ash

import math
import numpy
import colorsys

class ImageSignal(QObject):

   valueChanged = QtCore.Signal(ash.Image)

   def __init__(self, parent=None):
      super(ImageSignal, self).__init__(parent)

   def update(self, val):
      self.valueChanged.emit(val)

class ImageViewer(QtGui.QMainWindow):
   def __init__(self):
      super(ImageViewer, self).__init__()

      self.printer = QtGui.QPrinter()
      self.scaleFactor = 0.0

      # Channel indices and signal/slot for loading next image
      self.nextImage = ImageSignal()
      self.nextImage.valueChanged.connect(self.updateImage)

      self.glviewer = GLViewer()

      self.glviewer.setSizePolicy(QtGui.QSizePolicy.Ignored,
            QtGui.QSizePolicy.Ignored)
      self.glviewer.setStyleSheet("background-color:black")

      self.setCentralWidget(self.glviewer)

      self.createActions()
      self.createMenus()

      self.controls = ControlsWidget()

      self.controls.gamma.valueChanged.connect(self.gammaChanged)
      self.controls.brightness.valueChanged.connect(self.brightnessChanged)

      # Monitor channel selectors
      self.controls.redChannel.valueChanged.connect(self.redChannelChanged)
      self.controls.greenChannel.valueChanged.connect(self.greenChannelChanged)
      self.controls.blueChannel.valueChanged.connect(self.blueChannelChanged)

      self.setWindowTitle("Skipper")
      self.resize(800, 600)

   # New image ready to view
   @QtCore.Slot(ash.Image)
   def updateImage(self, image):
      self.glviewer.setImage(image)
      self.controls.buildChannelSelectors(image.Depth())
      print("updateImage")

   # Set what channel will be used for red, green and blue
   @QtCore.Slot(int)
   def redChannelChanged(self, channel):
      self.glviewer.updateRedChannel(channel)
      print("redChannelChanged")

   @QtCore.Slot(int)
   def greenChannelChanged(self, channel):
      self.glviewer.updateGreenChannel(channel)
      print("greenChannelChanged")

   @QtCore.Slot(int)
   def blueChannelChanged(self, channel):
      self.glviewer.updateBlueChannel(channel)
      print("blueChannelChanged")

   # Image manip. slots
   @QtCore.Slot(float)
   def gammaChanged(self, factor):
      self.glviewer.updateGamma(factor)
      print("")

   @QtCore.Slot(float)
   def brightnessChanged(self, factor):
      self.glviewer.updateBrightness(factor)
      print("")

   def openFromDisk(self):
      fileName,_ = QtGui.QFileDialog.getOpenFileName(self, "Open File",
         QtCore.QDir.currentPath())
      if fileName:
         test_if_image = QImage(fileName)
         if not test_if_image.isNull():
            self.glviewer.loadImageFromDisk(fileName)
            self.resize(test_if_image.width(), test_if_image.height())

            self.controls.setCurrentImageFilename(fileName.split("/")[-1]);
            self.controls.buildChannelSelectors(self.glviewer.image.Depth())

   def setImage(self, image):
      self.nextImage.update(image)

   def showControls(self):
      self.controls.show()

   def about(self):
      pass

   def shutdown(self):
      self.controls.close()
      self.close()

   def createActions(self):
      self.open_from_disk_action = QtGui.QAction("&Open from disk", self,
            triggered=self.openFromDisk)

      self.exit_action = QtGui.QAction("E&xit", self, shortcut="Ctrl+Q",
            triggered=self.shutdown)

      self.show_controls_action = QtGui.QAction("&Controls", self,
            triggered=self.showControls)

      self.about_action = QtGui.QAction("&About", self, triggered=self.about)

      self.about_Qt_action = QtGui.QAction("About &Qt", self,
            triggered=QtGui.qApp.aboutQt)

   def createMenus(self):
      self.fileMenu = QtGui.QMenu("&File", self)
      self.fileMenu.addAction(self.open_from_disk_action)
      self.fileMenu.addSeparator()
      self.fileMenu.addAction(self.exit_action)

      self.viewMenu = QtGui.QMenu("&View", self)
      self.viewMenu.addAction(self.show_controls_action)

      self.helpMenu = QtGui.QMenu("&Help", self)
      self.helpMenu.addAction(self.about_action)
      self.helpMenu.addAction(self.about_Qt_action)

      self.menuBar().addMenu(self.fileMenu)
      self.menuBar().addMenu(self.viewMenu)
      self.menuBar().addMenu(self.helpMenu)

   def adjustScrollBar(self, scrollBar, factor):
      scrollBar.setValue(int(factor * scrollBar.value()
                        + ((factor - 1) * scrollBar.pageStep()/2)))


if __name__ == '__main__':

   import sys

   app = QtGui.QApplication(sys.argv)
   imageViewer = ImageViewer()
   imageViewer.show()
   sys.exit(app.exec_())

