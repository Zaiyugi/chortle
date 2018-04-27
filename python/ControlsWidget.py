#!/usr/bin/python

from PySide import QtCore
from PySide.QtCore import *
from PySide.QtGui import *
import sys

class FloatSignal(QObject):

   valueChanged = QtCore.Signal(float)

   def __init__(self, parent=None):
      super(FloatSignal, self).__init__(parent)

   def update(self, val):
      self.valueChanged.emit(val)

class IntSignal(QObject):

   valueChanged = QtCore.Signal(int)

   def __init__(self, parent=None):
      super(IntSignal, self).__init__(parent)

   def update(self, val):
      self.valueChanged.emit(val)

class TupleSignal(QObject):

   valueChanged = QtCore.Signal(tuple)

   def __init__(self, parent=None):
      super(TupleSignal, self).__init__(parent)

   def update(self, val):
      self.valueChanged.emit(val)

class ControlsWidget(QWidget):

   def __init__(self, parent=None):
      super(ControlsWidget, self).__init__(parent)

      self.fractal_iters = IntSignal()
      self.raymarch_iters = IntSignal()

      self.fractal_scale = FloatSignal()
      
      self.flimit_x = FloatSignal()
      self.flimit_y = FloatSignal()
      self.flimit_z = FloatSignal()
      
      self.julia_C_x = FloatSignal()
      self.julia_C_y = FloatSignal()
      self.julia_C_z = FloatSignal()
      
      self.julia_offset_x = FloatSignal()
      self.julia_offset_y = FloatSignal()
      self.julia_offset_z = FloatSignal()

      self.de_offset = FloatSignal()

      self.minRadius2 = FloatSignal()
      self.fixedRadius2 = FloatSignal()
      self.glow_strength = FloatSignal()
      self.epsilon = FloatSignal()

      self.initUI()
      self.createActions()

   def initUI(self):

      self.line_separators = []

      # Raymarch Iterations
      self.raymarch_iters_steps = 2048
      self.raymarch_iters_label = QLabel('Raymarch Iterations:')
      self.raymarch_iters_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.raymarch_iters_field = QLineEdit("32")
      self.raymarch_iters_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.raymarch_iters_field.setMaximumWidth(100)

      self.raymarch_iters_slider = QSlider(Qt.Horizontal)
      self.raymarch_iters_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.raymarch_iters_slider.setRange(1, self.raymarch_iters_steps)

      # Fractal Iterations: connect signals
      self.raymarch_iters_field.returnPressed.connect(self.setRaymarchIterations)
      self.raymarch_iters_slider.valueChanged.connect(self.updateRaymarchIterations)

      self.setRaymarchIterations()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # Fractal Iterations
      self.fractal_iters_steps = 20
      self.fractal_iters_label = QLabel('Fractal Iterations:')
      self.fractal_iters_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.fractal_iters_field = QLineEdit("8")
      self.fractal_iters_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.fractal_iters_field.setMaximumWidth(100)

      self.fractal_iters_slider = QSlider(Qt.Horizontal)
      self.fractal_iters_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.fractal_iters_slider.setRange(1, self.fractal_iters_steps)

      # Fractal Iterations: connect signals
      self.fractal_iters_field.returnPressed.connect(self.setFractalIterations)
      self.fractal_iters_slider.valueChanged.connect(self.updateFractalIterations)

      self.setFractalIterations()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # Fractal Scale
      self.fractal_scale_steps = 10000
      self.fractal_scale_label = QLabel('Scale:')
      self.fractal_scale_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.fractal_scale_field = QLineEdit("2.0")
      self.fractal_scale_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.fractal_scale_field.setMaximumWidth(100)

      self.fractal_scale_slider = QSlider(Qt.Horizontal)
      self.fractal_scale_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.fractal_scale_slider.setRange(0, self.fractal_scale_steps)

      # Fractal Scale: connect signals
      self.fractal_scale_field.returnPressed.connect(self.setFractalScale)
      self.fractal_scale_slider.valueChanged.connect(self.updateFractalScale)

      self.setFractalScale()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # FLimit X (FLX)
      self.flimit_x_steps = 100
      self.flimit_x_label = QLabel('FLimit X:')
      self.flimit_x_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.flimit_x_field = QLineEdit("1.0")
      self.flimit_x_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.flimit_x_field.setMaximumWidth(100)

      self.flimit_x_slider = QSlider(Qt.Horizontal)
      self.flimit_x_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.flimit_x_slider.setRange(0, self.flimit_x_steps)

      # FLimit X: connect signals
      self.flimit_x_field.returnPressed.connect(self.setFLimitX)
      self.flimit_x_slider.valueChanged.connect(self.updateFLimitX)

      self.setFLimitX()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # FLimit Y (FLY)
      self.flimit_y_steps = 100
      self.flimit_y_label = QLabel('FLimit Y:')
      self.flimit_y_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.flimit_y_field = QLineEdit("1.0")
      self.flimit_y_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.flimit_y_field.setMaximumWidth(100)

      self.flimit_y_slider = QSlider(Qt.Horizontal)
      self.flimit_y_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.flimit_y_slider.setRange(0, self.flimit_y_steps)

      # FLimit Y: connect signals
      self.flimit_y_field.returnPressed.connect(self.setFLimitY)
      self.flimit_y_slider.valueChanged.connect(self.updateFLimitY)

      self.setFLimitY()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # FLimit Z (FLZ)
      self.flimit_z_steps = 100
      self.flimit_z_label = QLabel('FLimit Z:')
      self.flimit_z_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.flimit_z_field = QLineEdit("1.0")
      self.flimit_z_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.flimit_z_field.setMaximumWidth(100)

      self.flimit_z_slider = QSlider(Qt.Horizontal)
      self.flimit_z_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.flimit_z_slider.setRange(0, self.flimit_z_steps)

      # FLimit Z: connect signals
      self.flimit_z_field.returnPressed.connect(self.setFLimitZ)
      self.flimit_z_slider.valueChanged.connect(self.updateFLimitZ)

      self.setFLimitZ()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # minRadius2 (MR2)
      self.minRadius2_steps = 100
      self.minRadius2_label = QLabel('Min Radius:')
      self.minRadius2_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.minRadius2_field = QLineEdit("0.1")
      self.minRadius2_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.minRadius2_field.setMaximumWidth(100)

      self.minRadius2_slider = QSlider(Qt.Horizontal)
      self.minRadius2_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.minRadius2_slider.setRange(0, self.minRadius2_steps)

      # MR2: connect signals
      self.minRadius2_field.returnPressed.connect(self.setMR2)
      self.minRadius2_slider.valueChanged.connect(self.updateMR2)

      self.setMR2()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # fixedRadius2 (FR2)
      self.fixedRadius2_steps = 100
      self.fixedRadius2_label = QLabel('Fixed Radius:')
      self.fixedRadius2_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.fixedRadius2_field = QLineEdit("1.0")
      self.fixedRadius2_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.fixedRadius2_field.setMaximumWidth(100)

      self.fixedRadius2_slider = QSlider(Qt.Horizontal)
      self.fixedRadius2_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.fixedRadius2_slider.setRange(0, self.fixedRadius2_steps)

      # MR2: connect signals
      self.fixedRadius2_field.returnPressed.connect(self.setFR2)
      self.fixedRadius2_slider.valueChanged.connect(self.updateFR2)

      self.setFR2()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # glow_strength
      self.glow_strength_steps = 100
      self.glow_strength_label = QLabel('Glow Strength:')
      self.glow_strength_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.glow_strength_field = QLineEdit("0.5")
      self.glow_strength_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.glow_strength_field.setMaximumWidth(100)

      self.glow_strength_slider = QSlider(Qt.Horizontal)
      self.glow_strength_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.glow_strength_slider.setRange(0, self.glow_strength_steps)

      # glow_strength: connect signals
      self.glow_strength_field.returnPressed.connect(self.setGlowStrength)
      self.glow_strength_slider.valueChanged.connect(self.updateGlowStrength)

      self.setGlowStrength()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # Epsilon
      self.epsilon_steps = 100000
      self.epsilon_label = QLabel('Detail:')
      self.epsilon_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.epsilon_field = QLineEdit("0.0001")
      self.epsilon_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.epsilon_field.setMaximumWidth(100)

      self.epsilon_slider = QSlider(Qt.Horizontal)
      self.epsilon_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.epsilon_slider.setRange(0, self.epsilon_steps)

      # Fractal Scale: connect signals
      self.epsilon_field.returnPressed.connect(self.setEpsilon)
      self.epsilon_slider.valueChanged.connect(self.updateEpsilon)

      self.setEpsilon()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # Julia Cx
      self.julia_C_x_steps = 1000
      self.julia_C_x_label = QLabel('Julia Cx:')
      self.julia_C_x_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.julia_C_x_field = QLineEdit("1.0")
      self.julia_C_x_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.julia_C_x_field.setMaximumWidth(100)

      self.julia_C_x_slider = QSlider(Qt.Horizontal)
      self.julia_C_x_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.julia_C_x_slider.setRange(0, self.julia_C_x_steps)

      # Julia Cx: connect signals
      self.julia_C_x_field.returnPressed.connect(self.setJuliaCx)
      self.julia_C_x_slider.valueChanged.connect(self.updateJuliaCx)
      self.setJuliaCx()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # Julia Cy
      self.julia_C_y_steps = 1000
      self.julia_C_y_label = QLabel('Julia Cy:')
      self.julia_C_y_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.julia_C_y_field = QLineEdit("1.0")
      self.julia_C_y_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.julia_C_y_field.setMaximumWidth(100)

      self.julia_C_y_slider = QSlider(Qt.Horizontal)
      self.julia_C_y_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.julia_C_y_slider.setRange(0, self.julia_C_y_steps)

      # Julia Cx: connect signals
      self.julia_C_y_field.returnPressed.connect(self.setJuliaCy)
      self.julia_C_y_slider.valueChanged.connect(self.updateJuliaCy)
      self.setJuliaCy()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # Julia Cz
      self.julia_C_z_steps = 1000
      self.julia_C_z_label = QLabel('Julia Cz:')
      self.julia_C_z_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.julia_C_z_field = QLineEdit("1.0")
      self.julia_C_z_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.julia_C_z_field.setMaximumWidth(100)

      self.julia_C_z_slider = QSlider(Qt.Horizontal)
      self.julia_C_z_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.julia_C_z_slider.setRange(0, self.julia_C_z_steps)

      # Julia Cz: connect signals
      self.julia_C_z_field.returnPressed.connect(self.setJuliaCz)
      self.julia_C_z_slider.valueChanged.connect(self.updateJuliaCz)
      self.setJuliaCz()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # Julia Offset (X)
      self.julia_offset_x_steps = 1000
      self.julia_offset_x_label = QLabel('Julia Offset (X):')
      self.julia_offset_x_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.julia_offset_x_field = QLineEdit("1.0")
      self.julia_offset_x_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.julia_offset_x_field.setMaximumWidth(100)

      self.julia_offset_x_slider = QSlider(Qt.Horizontal)
      self.julia_offset_x_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.julia_offset_x_slider.setRange(0, self.julia_offset_x_steps)

      # Julia Offset (Y): connect signals
      self.julia_offset_x_field.returnPressed.connect(self.setJuliaOffsetX)
      self.julia_offset_x_slider.valueChanged.connect(self.updateJuliaOffsetX)
      self.setJuliaOffsetX()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # Julia Offset (Y)
      self.julia_offset_y_steps = 1000
      self.julia_offset_y_label = QLabel('Julia Offset (Y):')
      self.julia_offset_y_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.julia_offset_y_field = QLineEdit("1.0")
      self.julia_offset_y_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.julia_offset_y_field.setMaximumWidth(100)

      self.julia_offset_y_slider = QSlider(Qt.Horizontal)
      self.julia_offset_y_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.julia_offset_y_slider.setRange(0, self.julia_offset_y_steps)

      # Julia Offset (Y): connect signals
      self.julia_offset_y_field.returnPressed.connect(self.setJuliaOffsetY)
      self.julia_offset_y_slider.valueChanged.connect(self.updateJuliaOffsetY)
      self.setJuliaOffsetY()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # Julia Offset (Z)
      self.julia_offset_z_steps = 1000
      self.julia_offset_z_label = QLabel('Julia Offset (Z):')
      self.julia_offset_z_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.julia_offset_z_field = QLineEdit("1.0")
      self.julia_offset_z_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.julia_offset_z_field.setMaximumWidth(100)

      self.julia_offset_z_slider = QSlider(Qt.Horizontal)
      self.julia_offset_z_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.julia_offset_z_slider.setRange(0, self.julia_offset_z_steps)

      # Julia Offset (Z): connect signals
      self.julia_offset_z_field.returnPressed.connect(self.setJuliaOffsetZ)
      self.julia_offset_z_slider.valueChanged.connect(self.updateJuliaOffsetZ)
      self.setJuliaOffsetZ()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      # DE Offset
      self.de_offset_steps = 1000
      self.de_offset_label = QLabel('DE Offset:')
      self.de_offset_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

      self.de_offset_field = QLineEdit("0.0")
      self.de_offset_field.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
      self.de_offset_field.setMaximumWidth(100)

      self.de_offset_slider = QSlider(Qt.Horizontal)
      self.de_offset_slider.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
      self.de_offset_slider.setRange(0, self.de_offset_steps)

      # Julia Offset (Z): connect signals
      self.de_offset_field.returnPressed.connect(self.setDEOffset)
      self.de_offset_slider.valueChanged.connect(self.updateDEOffset)
      self.setDEOffset()

      line = QFrame()
      line.setFrameShape(QFrame.HLine)
      line.setFrameShadow(QFrame.Sunken)
      self.line_separators.append(line)

      ### Layout ###

      grid = QGridLayout()

      # Raymarch Iterations
      grid.addWidget(self.raymarch_iters_label, 0, 0)
      grid.addWidget(self.raymarch_iters_field, 0, 1)
      grid.addWidget(self.raymarch_iters_slider, 1, 0, 1, 3)
      grid.addWidget(self.line_separators[0], 2, 0, 1, 3)

      # Fractal Iterations
      grid.addWidget(self.fractal_iters_label, 3, 0)
      grid.addWidget(self.fractal_iters_field, 3, 1)
      grid.addWidget(self.fractal_iters_slider, 4, 0, 1, 3)
      grid.addWidget(self.line_separators[1], 5, 0, 1, 3)

      # Fractal Scale
      grid.addWidget(self.fractal_scale_label, 6, 0)
      grid.addWidget(self.fractal_scale_field, 6, 1)
      grid.addWidget(self.fractal_scale_slider, 7, 0, 1, 3)
      grid.addWidget(self.line_separators[2], 8, 0, 1, 3)

      # FLimit X
      grid.addWidget(self.flimit_x_label, 9, 0)
      grid.addWidget(self.flimit_x_field, 9, 1)
      grid.addWidget(self.flimit_x_slider, 10, 0, 1, 3)
      grid.addWidget(self.line_separators[3], 11, 0, 1, 3)

      # FLimit Y
      grid.addWidget(self.flimit_y_label, 12, 0)
      grid.addWidget(self.flimit_y_field, 12, 1)
      grid.addWidget(self.flimit_y_slider, 13, 0, 1, 3)
      grid.addWidget(self.line_separators[4], 14, 0, 1, 3)

      # FLimit Z
      grid.addWidget(self.flimit_z_label, 15, 0)
      grid.addWidget(self.flimit_z_field, 15, 1)
      grid.addWidget(self.flimit_z_slider, 16, 0, 1, 3)
      grid.addWidget(self.line_separators[5], 17, 0, 1, 3)

      # minRadius2
      grid.addWidget(self.minRadius2_label, 18, 0)
      grid.addWidget(self.minRadius2_field, 18, 1)
      grid.addWidget(self.minRadius2_slider, 19, 0, 1, 3)
      grid.addWidget(self.line_separators[6], 20, 0, 1, 3)

      # fixedRadius2
      grid.addWidget(self.fixedRadius2_label, 21, 0)
      grid.addWidget(self.fixedRadius2_field, 21, 1)
      grid.addWidget(self.fixedRadius2_slider, 22, 0, 1, 3)
      grid.addWidget(self.line_separators[7], 23, 0, 1, 3)

      # glow_strength
      grid.addWidget(self.glow_strength_label, 24, 0)
      grid.addWidget(self.glow_strength_field, 24, 1)
      grid.addWidget(self.glow_strength_slider, 25, 0, 1, 3)
      grid.addWidget(self.line_separators[8], 26, 0, 1, 3)

      # epsilon
      grid.addWidget(self.epsilon_label, 27, 0)
      grid.addWidget(self.epsilon_field, 27, 1)
      grid.addWidget(self.epsilon_slider, 28, 0, 1, 3)
      grid.addWidget(self.line_separators[9], 29, 0, 1, 3)

      # Julia Cx
      grid.addWidget(self.julia_C_x_label, 30, 0)
      grid.addWidget(self.julia_C_x_field, 30, 1)
      grid.addWidget(self.julia_C_x_slider, 31, 0, 1, 3)
      grid.addWidget(self.line_separators[10], 32, 0, 1, 3)

      # Julia Cy
      grid.addWidget(self.julia_C_y_label, 33, 0)
      grid.addWidget(self.julia_C_y_field, 33, 1)
      grid.addWidget(self.julia_C_y_slider, 34, 0, 1, 3)
      grid.addWidget(self.line_separators[11], 35, 0, 1, 3)

      # Julia Cz
      grid.addWidget(self.julia_C_z_label, 36, 0)
      grid.addWidget(self.julia_C_z_field, 36, 1)
      grid.addWidget(self.julia_C_z_slider, 37, 0, 1, 3)
      grid.addWidget(self.line_separators[12], 38, 0, 1, 3)

      # Julia Offset (X)
      grid.addWidget(self.julia_offset_x_label, 39, 0)
      grid.addWidget(self.julia_offset_x_field, 39, 1)
      grid.addWidget(self.julia_offset_x_slider, 40, 0, 1, 3)
      grid.addWidget(self.line_separators[13], 41, 0, 1, 3)

      # Julia Offset (Y)
      grid.addWidget(self.julia_offset_y_label, 42, 0)
      grid.addWidget(self.julia_offset_y_field, 42, 1)
      grid.addWidget(self.julia_offset_y_slider, 43, 0, 1, 3)
      grid.addWidget(self.line_separators[14], 44, 0, 1, 3)

      # Julia Offset (Z)
      grid.addWidget(self.julia_offset_z_label, 45, 0)
      grid.addWidget(self.julia_offset_z_field, 45, 1)
      grid.addWidget(self.julia_offset_z_slider, 46, 0, 1, 3)
      grid.addWidget(self.line_separators[15], 47, 0, 1, 3)

      # DE Offset
      grid.addWidget(self.de_offset_label, 48, 0)
      grid.addWidget(self.de_offset_field, 48, 1)
      grid.addWidget(self.de_offset_slider, 49, 0, 1, 3)
      grid.addWidget(self.line_separators[16], 50, 0, 1, 3)

      self.setLayout(grid)

      self.setWindowTitle('Controls')
      self.resize(300, 1000)

   def createActions(self):
      self.exit_action = QAction("Exit", self, triggered=self.close)

   #############
   ### Slots ###
   #############

   ###
   def setRaymarchIterations(self):
      value = int(self.raymarch_iters_field.text())
      self.raymarch_iters_slider.setValue(value)

   # Raymarch Iterations
   def updateRaymarchIterations(self):
      value = int(self.raymarch_iters_slider.value())
      self.raymarch_iters_field.setText(str(value))

      # Send raymarch iterations signal
      self.raymarch_iters.update(value)
   ###

   ###
   def setFractalIterations(self):
      value = int(self.fractal_iters_field.text())
      self.fractal_iters_slider.setValue(value)

   # Fractal Iterations
   def updateFractalIterations(self):
      value = int(self.fractal_iters_slider.value())
      self.fractal_iters_field.setText(str(value))

      # Send fractal iterations signal
      self.fractal_iters.update(value)
   ###

   ###
   def setFractalScale(self):
      value = float(self.fractal_scale_field.text())
      half = self.fractal_scale_steps / 2
      span = 2.0
      if value < -1.0:
         value = abs(value + 1.0) / span * half
      elif value > 1.0:
         value = abs(value - 1.0) / span * half + half

      self.fractal_scale_slider.setValue(value)

   # Fractal Scale
   def updateFractalScale(self):
      value = float(self.fractal_scale_slider.value()) / self.fractal_scale_steps
      span = 2.0
      if value <= 0.5:
         value = 0.5 - value
         value = (2.0 * value) * -span - 1.0
      elif value > 0.5:
         value -= 0.5
         value = (2.0 * value) * span + 1.0

      self.fractal_scale_field.setText(str(value))

      # Send fractal scale signal
      self.fractal_scale.update(value)
   ###

   ###
   def setFLimitX(self):
      value = float(self.flimit_x_field.text());

      value = (value - 0.0001) / (3.0 - 0.0001)
      value *= self.flimit_x_steps

      self.flimit_x_slider.setValue(value)

   def updateFLimitX(self):
      value = float(self.flimit_x_slider.value()) / self.flimit_x_steps
      value = value * (3.0 - 0.0001) + 0.0001

      self.flimit_x_field.setText(str(value))

      # Send flimit_x signal
      self.flimit_x.update(value)
   ###

   ###
   def setFLimitY(self):
      value = float(self.flimit_y_field.text());

      value = (value - 0.0001) / (3.0 - 0.0001)
      value *= self.flimit_y_steps

      self.flimit_y_slider.setValue(value)

   def updateFLimitY(self):
      value = float(self.flimit_y_slider.value()) / self.flimit_y_steps
      value = value * (3.0 - 0.0001) + 0.0001

      self.flimit_y_field.setText(str(value))

      # Send flimit_y signal
      self.flimit_y.update(value)
   ###

   ###
   def setFLimitZ(self):
      value = float(self.flimit_z_field.text());

      value = (value - 0.0001) / (3.0 - 0.0001)
      value *= self.flimit_z_steps

      self.flimit_z_slider.setValue(value)

   def updateFLimitZ(self):
      value = float(self.flimit_z_slider.value()) / self.flimit_z_steps
      value = value * (3.0 - 0.0001) + 0.0001

      self.flimit_z_field.setText(str(value))

      # Send flimit_z signal
      self.flimit_z.update(value)
   ###

   ###
   def setMR2(self):
      value = float(self.minRadius2_field.text())
      value *= self.minRadius2_steps
      self.minRadius2_slider.setValue(value)

   # minRadius2
   def updateMR2(self):
      value = float(self.minRadius2_slider.value()) / self.minRadius2_steps
      self.minRadius2_field.setText(str(value))

      # Send minRadius2 signal
      self.minRadius2.update(value)
   ###

   ###
   def setFR2(self):
      value = float(self.fixedRadius2_field.text())

      value = (value - 0.0001) / (3.0 - 0.0001)
      value *= self.fixedRadius2_steps

      self.fixedRadius2_slider.setValue(value)

   # fixedRadius2
   def updateFR2(self):
      value = float(self.fixedRadius2_slider.value()) / self.fixedRadius2_steps
      value = value * (3.0 - 0.0001) + 0.0001

      self.fixedRadius2_field.setText(str(value))

      # Send fixedRadius2 signal
      self.fixedRadius2.update(value)
   ###

   ###
   def setGlowStrength(self):
      value = float(self.glow_strength_field.text()) / 2.0;
      value *= self.glow_strength_steps

      self.glow_strength_slider.setValue(value)

   # Glow Strength
   def updateGlowStrength(self):
      value = float(self.glow_strength_slider.value()) / self.glow_strength_steps
      value *= 2.0

      self.glow_strength_field.setText(str(value))

      # Send glow strength signal
      self.glow_strength.update(value)
   ###

   ###
   def setEpsilon(self):
      value = float(self.epsilon_field.text());

      value = (value - 0.000001) / (0.0001 - 0.000001)
      value *= self.epsilon_steps

      self.epsilon_slider.setValue(value)
   # Epsilon

   def updateEpsilon(self):
      value = float(self.epsilon_slider.value()) / self.epsilon_steps
      value = value * (0.0001 - 0.000001) + 0.000001

      self.epsilon_field.setText(str(value))

      # Send epsilon signal
      self.epsilon.update(value)
   ###

   ###
   def setJuliaCx(self):
      value = float(self.julia_C_x_field.text());

      value = (value - -2.0) / (2.0 - -2.0)
      value *= self.julia_C_x_steps

      self.julia_C_x_slider.setValue(value)

   def updateJuliaCx(self):
      value = float(self.julia_C_x_slider.value()) / self.julia_C_x_steps
      value = value * (2.0 - -2.0) + -2.0

      self.julia_C_x_field.setText(str(value))

      # Send julia_C_x signal
      self.julia_C_x.update(value)
   ###

   ###
   def setJuliaCy(self):
      value = float(self.julia_C_y_field.text());

      value = (value - -2.0) / (2.0 - -2.0)
      value *= self.julia_C_y_steps

      self.julia_C_y_slider.setValue(value)

   def updateJuliaCy(self):
      value = float(self.julia_C_y_slider.value()) / self.julia_C_y_steps
      value = value * (2.0 - -2.0) + -2.0

      self.julia_C_y_field.setText(str(value))

      # Send julia_C_y signal
      self.julia_C_y.update(value)
   ###

   ###
   def setJuliaCz(self):
      value = float(self.julia_C_z_field.text());

      value = (value - -2.0) / (2.0 - -2.0)
      value *= self.julia_C_z_steps

      self.julia_C_z_slider.setValue(value)

   def updateJuliaCz(self):
      value = float(self.julia_C_z_slider.value()) / self.julia_C_z_steps
      value = value * (2.0 - -2.0) + -2.0

      self.julia_C_z_field.setText(str(value))

      # Send julia_C_z signal
      self.julia_C_z.update(value)
   ###

   ###
   def setJuliaOffsetX(self):
      value = float(self.julia_offset_x_field.text());

      value = (value - -1.0) / (1.0 - -1.0)
      value *= self.julia_offset_x_steps

      self.julia_offset_x_slider.setValue(value)

   def updateJuliaOffsetX(self):
      value = float(self.julia_offset_x_slider.value()) / self.julia_offset_x_steps
      value = value * (1.0 - -1.0) + -1.0

      self.julia_offset_x_field.setText(str(value))

      # Send julia_offset_x signal
      self.julia_offset_x.update(value)
   ###

   ###
   def setJuliaOffsetY(self):
      value = float(self.julia_offset_y_field.text());

      value = (value - -1.0) / (1.0 - -1.0)
      value *= self.julia_offset_y_steps

      self.julia_offset_y_slider.setValue(value)

   def updateJuliaOffsetY(self):
      value = float(self.julia_offset_y_slider.value()) / self.julia_offset_y_steps
      value = value * (1.0 - -1.0) + -1.0

      self.julia_offset_y_field.setText(str(value))

      # Send julia_offset_y signal
      self.julia_offset_y.update(value)
   ###

   ###
   def setJuliaOffsetZ(self):
      value = float(self.julia_offset_z_field.text());

      value = (value - -1.0) / (1.0 - -1.0)
      value *= self.julia_offset_z_steps

      self.julia_offset_z_slider.setValue(value)

   def updateJuliaOffsetZ(self):
      value = float(self.julia_offset_z_slider.value()) / self.julia_offset_z_steps
      value = value * (1.0 - -1.0) + -1.0

      self.julia_offset_z_field.setText(str(value))

      # Send julia_offset_z signal
      self.julia_offset_z.update(value)
   ###

   ###
   def setDEOffset(self):
      value = float(self.de_offset_field.text());

      value = value / 0.5
      value *= self.de_offset_steps


      self.de_offset_slider.setValue(value)

   def updateDEOffset(self):
      value = float(self.de_offset_slider.value()) / self.de_offset_steps
      value *= 0.5

      self.de_offset_field.setText(str(value))

      # Send de_offset signal
      self.de_offset.update(value)
   ###

   ###############
   ### Utility ###
   ###############

   ###############
   ### Getters ###
   ###############

   # End of class ControlsWidget

if __name__ == "__main__":
   app = QApplication(sys.argv)

   window = ControlsWidget()

   sys.exit(app.exec_())
