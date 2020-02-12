#!/usr/bin/env python
# -*- coding: utf-8 -*-

import Sofa

# see SOFA_DIR/src/applications/plugins/SofaPython/examples/VectorLinearSpringData.py
# for demo of how to access data in springs.
 
class SMA_wireController(Sofa.PythonScriptController):
    def initGraph(self, node):
        print 'initGraph SMA_wireController' 
        self.node = node

    def onKeyPressed(self,c):
        if (c == "+"):
            print 'heating'
            wire = self.node.getObject('actuator_wire')
            ls = Sofa.LinearSpring(0,0,0,0,0)  #(0, 1, 500., 5., 100.)
            for i in range (0, len(wire.spring)):
                ls = wire.spring[i]            # insert SMA heating algorithm here
                ls.L *=0.95
                ls.Ks +=100
                wire.spring[i] = ls
            #wire.stiffness *=10 #stiffness is controlled for the whole wire, not segmentwise
            #self.node.getObject('actuator_wire').reinit()
            print wire.spring    

        elif (c == "-"):
            print 'cooling'
            wire = self.node.getObject('actuator_wire')
            ls = Sofa.LinearSpring(0,0,0,0,0)#(0, 1, 500., 5., 100.)
            for i in range (0, len(wire.spring)):
                ls = wire.spring[i]            # insert SMA cooling algorithm here
                ls.L /=0.95
                ls.Ks -=100
                wire.spring[i] = ls
            #self.node.getObject('actuator_wire').reinit()
            print wire.spring 
            
        sys.stdout.flush()
        return 0  
 
 #NB eventually the input should be electrical power
 #The heating, cooling by diffusion and SMA phase change should happen on visitors that operate every time step.
 
 
class FEMcontroller(Sofa.PythonScriptController):
     
    def initGraph(self, node):
        self.node = node
        print 'initGraph FEMcontroller' 
         
    def onLoaded(self,node):
        print 'onLoaded FEMcontroller'
         
    def createGraph(self,node):
        print 'createGraph called FEMcontroller'
    
    def onKeyPressed(self,k):
        print 'onKeyPressed '
        if ord(k)==19:
            print ' stiffen ' #UP key
            print (self.node.getObject('FEM').findData('youngModulus').value)
            self.node.getObject('FEM').findData('youngModulus').setValue(0,180000.0)
            self.node.getObject('FEM').reinit()
            print (self.node.getObject('FEM').findData('youngModulus'))
        elif ord(k)==21:
            print ' soften ' #DOWN key
            print (self.node.getObject('FEM').findData('youngModulus'))
            self.node.getObject('FEM').findData('youngModulus').setValue(0,18000.0)
            self.node.getObject('FEM').reinit()
            print (self.node.getObject('FEM').findData('youngModulus'))
        elif (k== "M"):
            print 'localStiffnessFactor='
            #self.node.getObject('FEM').findData('localStiffnessFactor').setValue(0,1)
            #self.node.getObject('FEM').findData('localStiffnessFactor').setValue(1,10)
            self.node.getObject('FEM').reinit()
            print (self.node.getObject('FEM').findData('localStiffnessFactor'))
        sys.stdout.flush()
        return 0 
