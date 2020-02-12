# Notes on the SOFA Python API

## General Intro

https://www.sofa-framework.org/community/doc/using-sofa/optional-features/python-scripting/


## SofaPython Plugin

To use the Python API, Sofa must be compiled with the SofaPython option selected in CMake. (Both our Vbox and Singularity builds include SofaPython.)

The code of the SofaPython plugin is found at:

SOFA_DIR/src/applications/plugins/SofaPython
API manual

SOFA_DIR/src/applications/plugins/SofaPython/doc/SofaPython.pdf
Tutorial

SOFA_DIR/src/applications/plugins/SofaPython/doc/SofaDays_oct2013
Examples

SOFA_DIR/src/applications/plugins/SofaPython/examples
Important concepts
Python Scenes and Simulation Objects

The XML .scn scene files can be generated from Python .pyscn files.

#### These principally use the pattern:

	import sofa

	def createScene(rootNode):

    rootNode.createObject('..objectType..', name='..objectName..', otherParams='...')

   	..leaf nodes describe components ..

    ..nodeName.. = rootNode.createChild('..nodeName..')

        ..leaf nodes describe components ..

  	return rootNode


## Python Script Controllers

Script controllers are created by the following Python or XML lines

     finger.createObject('PythonScriptController', filename="controller/FingerControllerVisitorTest.py", classname="FEMcontroller")

     <PythonScriptController filename="ExampleController.py" classname="ExampleController" printLog="true"/>

Script controllers allow simulations to run scripts. These may enable user interaction via GUI events, or implement behaviours internal to the simulation.

#### The available event triggers are listed in

SOFA_DIR/src/applications/plugins/SofaPython/examples/ExampleController.py

and on page 15 of the Sofa Python API manual.


## Python Visitors

Visitors, whether C++ or Python, work their way around the nodes within their scope on the scene graph.

The following files describe a scene with a Python script controller that calls a Python visitor

SOFA_DIR/src/applications/plugins/SofaPython/examples/Visitor.scn

SOFA_DIR/src/applications/plugins/SofaPython/examples/Visitor.py

Also see the example in

SOFA_DIR/src/applications/plugins/SofaPython/doc/SofaDays_oct2013/6_Visitors

and page 14 of the Sofa Python API manual.


## Accessing data in simulation objects

This is done with

     node.findData('..nameOfData...').value

Sofa.Data has attributes 

    value
    name

and methods

    getValue(index)
    setValue(index,value)

See pages 10 & 12 of the Sofa Python API manual, and 


E.g. To modify the Young's Modulus of a FEM in response to GUI key presses: 

    def onKeyPressed(self,k):

        print 'onKeyPressed '+k

        if ord(k)==19:

            print ' stiffen ' #UP key

            print (self.node.getObject('FEM').findData('youngModulus').value)

            self.node.getObject('FEM').findData('youngModulus').setValue(0,180000.0)

            self.node.getObject('FEM').reinit()

       elif ord(k)==21:

            print ' soften ' #DOWN key

            print (self.node.getObject('FEM').findData('youngModulus')

            self.node.getObject('FEM').findData('youngModulus').setValue(0,18000.0)

            self.node.getObject('FEM').reinit()

            print (self.node.getObject('FEM').findData('youngModulus'))

        sys.stdout.flush()

        return 0

*****Note  .reinit()  is required to pass the update from the xml to the running simulation. ********


## Binding to the Python API

On writing bindings to the Python API for additional C++ classes, attributes, and methods

https://www.sofa-framework.org/community/doc/programming-with-sofa/start-coding/python-data-wrapping/

#### See the files

Binding.h, Binding.cpp, and Binding_*classname*.h, Binding_*classname*.cpp

in SOFA_DIR/src/applications/plugins/SofaPython





