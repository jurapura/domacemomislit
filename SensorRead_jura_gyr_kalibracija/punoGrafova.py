__author__ = 'Juraj'

import sys
from PyQt4 import QtCore, QtGui, uic
import numpy as np
import pyqtgraph as pg
import serial
from scipy import signal

class GUI():

    def __init__(self):
        self.timer = QtCore.QTimer()
        self.app = QtGui.QApplication(sys.argv)
        self.window = uic.loadUi("GuiGrafovi.ui")
        self.window.show()
        self.pocetno=0

        self.vrijeme=QtCore.QDateTime()

        self.window.textEdit.textChanged.connect(self.sta_daradi_kad_se_unese_text)

        pg.setConfigOptions(antialias=True)

        self.gyr_roll=0;
        self.gyr_pitch=0;
        self.gyr_roll_bias=-0.59
        self.gyr_pitch_bias=1.33
        self.dT=0.02

        self.xacc_graph=self.window.xacc_graph.plot(pen=(255,255,0))
        self.yacc_graph=self.window.yacc_graph.plot(pen=(255,0,0))
        self.zacc_graph=self.window.zacc_graph.plot(pen=(0,0,255))

        self.xgyr_graph=self.window.xgyr_graph.plot(pen=(255,255,0))
        self.ygyr_graph=self.window.ygyr_graph.plot(pen=(255,0,0))
        self.zgyr_graph=self.window.zgyr_graph.plot(pen=(0,0,255))

        self.xmag_graph=self.window.xmag_graph.plot(pen=(255,255,0))
        self.ymag_graph=self.window.ymag_graph.plot(pen=(255,0,0))
        self.zmag_graph=self.window.zmag_graph.plot(pen=(0,0,255))

        self.window.xmag_graph.setYRange(-0.13,0.13)
        self.window.ymag_graph.setYRange(-0.25,0.25)
        self.window.zmag_graph.setYRange(-0.13,0.13)

        self.window.xgyr_graph.setYRange(-200,200)
        self.window.ygyr_graph.setYRange(-200,200)
        self.window.zgyr_graph.setYRange(-200,200)

        self.window.xacc_graph.setYRange(-2,2)
        self.window.yacc_graph.setYRange(-2,2)
        self.window.zacc_graph.setYRange(-2,2)

        self.roll_graph=self.window.roll_graph.plot(pen=(255,255,0))
        self.pitch_graph=self.window.pitch_graph.plot(pen=(255,0,0))

        self.acc_roll_graph=self.window.roll_acc.plot(pen=(255,255,0))
        self.acc_pitch_graph=self.window.pitch_acc.plot(pen=(255,0,0))
        self.gyr_roll_graph=self.window.roll_gyr.plot(pen=(255,255,0))
        self.gyr_pitch_graph=self.window.pitch_gyr.plot(pen=(255,0,0))
        self.compl_roll_graph=self.window.roll_compl.plot(pen=(255,255,0))
        self.compl_pitch_graph=self.window.pitch_compl.plot(pen=(255,0,0))

        self.ser = serial.Serial(14,57600)  # open 15 serial port
        #self.ser.baudrate=57600;

        self.xacc_data=[0.0]*100
        self.yacc_data=[0.0]*100
        self.zacc_data=[0.0]*100
        self.xgyr_data=[0.0]*100
        self.ygyr_data=[0.0]*100
        self.zgyr_data=[0.0]*100
        self.xmag_data=[0.0]*100
        self.ymag_data=[0.0]*100
        self.zmag_data=[0.0]*100

        self.acc_roll_data=[0.0]*100
        self.acc_pitch_data=[0.0]*100
        self.gyr_roll_data=[0.0]*100
        self.gyr_pitch_data=[0.0]*100
        self.compl_roll_data=[0.0]*100
        self.compl_pitch_data=[0.0]*100

        self.time=np.linspace(-5, 0, 100)
        self.time2=np.linspace(-50, 0, 100)



        timer = QtCore.QTimer()
        timer.timeout.connect(self.update)
        timer.start(5)

        sys.exit(self.app.exec_())


    def sta_daradi_kad_se_unese_text(self):
            tekst = self.window.textEdit.toPlainText()

            self.window.lcdNumber.value = tekst
            self.window.lcdNumber.display(self.window.lcdNumber.value)

    def upisi_theta(self,theta):
             self.window.lcdNumber.value = theta
             self.window.lcdNumber.display(self.window.lcdNumber.value)

    def update(self):
        # x=np.linspace(0, 7, 1000)
        # y= np.sin(x+self.pocetno)
        # self.sinusic.setData(x,y)
        # self.pocetno=self.pocetno+0.1

        pom=self.ser.readline().replace("\r\n", "").split(',')
        imu_data=[float(i) for i in pom]

        #print self.vrijeme.currentMSecsSinceEpoch()
        #print imu_data[6]
        #print imu_data[7]


        # self.xacc_data=self.xacc_data[1:100]+[imu_data[0]]
        # self.yacc_data=self.yacc_data[1:100]+[imu_data[1]]
        # self.zacc_data=self.zacc_data[1:100]+[imu_data[2]]
        self.xgyr_data=self.xgyr_data[1:100]+[imu_data[0]]
        self.ygyr_data=self.ygyr_data[1:100]+[imu_data[1]]
        self.zgyr_data=self.zgyr_data[1:100]+[imu_data[2]]
        # self.xmag_data=self.xmag_data[1:100]+[imu_data[6]]
        # self.ymag_data=self.ymag_data[1:100]+[imu_data[8]]
        # self.zmag_data=self.zmag_data[1:100]+[imu_data[7]]

        # norma=imu_data[0]**2+imu_data[1]**2+imu_data[2]**2
        # gx=imu_data[0]/norma
        # gy=imu_data[1]/norma
        # gz=imu_data[2]/norma
        # roll=np.arctan2(gy,gz)*(180/3.14)
        # pitch=np.arctan2(-gx,np.sqrt(gy**2+gz**2))*(180/3.14)
        #
        # self.acc_roll_data=self.acc_roll_data[1:100]+[roll]
        # self.acc_pitch_data=self.acc_pitch_data[1:100]+[pitch]
        #
        # self.gyr_roll= self.gyr_roll+(imu_data[3]-self.gyr_roll_bias)*self.dT;
        # self.gyr_pitch=self.gyr_pitch+(imu_data[4]-self.gyr_pitch_bias)*self.dT;
        #
        # beta=0.95
        # self.compl_roll_data=self.compl_roll_data[1:100]+[beta*self.gyr_roll+(1-beta)*roll];
        # self.compl_pitch_data=self.compl_pitch_data[1:100]+[beta*self.gyr_pitch+(1-beta)*pitch];
        #
        # self.gyr_roll_data=self.gyr_roll_data[1:100]+[self.gyr_roll]
        # self.gyr_pitch_data=self.gyr_pitch_data[1:100]+[self.gyr_pitch]



#        self.xacc_data=scipy.signal.medfilt(self.xacc_data,5)

        # self.xacc_graph.setData(self.time,self.xacc_data)
        # self.yacc_graph.setData(self.time,self.yacc_data)
        # self.zacc_graph.setData(self.time,self.zacc_data)
        self.xgyr_graph.setData(self.time2,self.xgyr_data)
        self.ygyr_graph.setData(self.time2,self.ygyr_data)
        self.zgyr_graph.setData(self.time2,self.zgyr_data)
        # self.xmag_graph.setData(self.time,self.xmag_data)
        # self.ymag_graph.setData(self.time,self.ymag_data)
        # self.zmag_graph.setData(self.time,self.zmag_data)

        # self.roll_graph.setData(self.time,self.acc_roll_data)
        # self.pitch_graph.setData(self.time,self.acc_pitch_data)
        #
        # self.acc_roll_graph.setData(self.time,self.acc_roll_data)
        # self.acc_pitch_graph.setData(self.time,self.acc_pitch_data)
        #
        # self.gyr_roll_graph.setData(self.time,self.gyr_roll_data)
        # self.gyr_pitch_graph.setData(self.time,self.gyr_pitch_data)
        #
        # self.compl_roll_graph.setData(self.time,self.compl_roll_data)
        # self.compl_pitch_graph.setData(self.time,self.compl_pitch_data)

        xgyrsr = np.array(self.xgyr_data)
        self.window.xgyrsr.value = np.mean(xgyrsr)
        self.window.xgyrsr.display(self.window.xgyrsr.value)

        ygyrsr = np.array(self.ygyr_data)
        self.window.ygyrsr.value = np.mean(ygyrsr)
        self.window.ygyrsr.display(self.window.ygyrsr.value)

        zgyrsr = np.array(self.zgyr_data)
        self.window.zgyrsr.value = np.mean(zgyrsr)
        self.window.zgyrsr.display(self.window.zgyrsr.value)




if __name__ == "__main__":
    a = GUI()