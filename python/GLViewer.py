#!/usr/bin/env python

from PySide import QtCore, QtGui, QtOpenGL

from chortle import *

class GLViewer(QtOpenGL.QGLWidget):

   def __init__(self, width = 1920, height = 1080):
      super(GLViewer, self).__init__()

      #self.renderer = OGLRenderer(720, 720)
      self.renderer = OGLRenderer(width, height)

      self.makeCurrent()
      self.renderer.init()

   def initializeGL(self):
      # All initialization is handle by OGLRenderer
      pass

   def handleMouseButtonEvent(self, button, action, mods, x, y):
      self.renderer.mouseButtonCallback(button, action, mods, x, y)
      self.updateGL()

   def handleMouseMoveEvent(self, x, y):
      self.renderer.cursorPosCallback(x, y)
      self.updateGL()

   def setRaymarchIterations(self, val):
      self.renderer.setRaymarchIterations(val)
      self.updateGL()

   def setFractalIterations(self, val):
      self.renderer.setFractalIterations(val)
      self.updateGL()

   def setFractalScale(self, val):
      self.renderer.setFractalScale(val)
      self.updateGL()

   def setFLimitX(self, val):
      self.renderer.setFLimitX(val)
      self.updateGL()

   def setFLimitY(self, val):
      self.renderer.setFLimitY(val)
      self.updateGL()

   def setFLimitZ(self, val):
      self.renderer.setFLimitZ(val)
      self.updateGL()

   def setJuliaCx(self, val):
      self.renderer.setJuliaCx(val)
      self.updateGL()

   def setJuliaCy(self, val):
      self.renderer.setJuliaCy(val)
      self.updateGL()

   def setJuliaCz(self, val):
      self.renderer.setJuliaCz(val)
      self.updateGL()

   def setJuliaOffsetX(self, val):
      self.renderer.setJuliaOffsetX(val)
      self.updateGL()

   def setJuliaOffsetY(self, val):
      self.renderer.setJuliaOffsetY(val)
      self.updateGL()

   def setJuliaOffsetZ(self, val):
      self.renderer.setJuliaOffsetZ(val)
      self.updateGL()

   def setDEOffset(self, val):
      self.renderer.setDEOffset(val)
      self.updateGL()

   def setMR2(self, val):
      self.renderer.setMR2(val)
      self.updateGL()

   def setFR2(self, val):
      self.renderer.setFR2(val)
      self.updateGL()

   def setGlowStrength(self, val):
      self.renderer.setGlowStrength(val)
      self.updateGL()

   def setEpsilon(self, val):
      self.renderer.setEpsilon(val)
      self.updateGL()

   def getConfiguration(self):
      self.renderer.getConfiguration()

   # def setUniforms(self):
   #    self.updateGL()

   def resizeGL(self, w, h):
      #print(w,h)

      self.widget_width = w
      self.widget_height = h

      self.renderer.reshapeCallback(w, h)

   def paintGL(self):

      self.renderer.draw()

      self.swapBuffers()

   # End GLViewer
