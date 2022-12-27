#The project files are available under a  CC-BY 4.0 license,
Please refer to the license statement here:
https://creativecommons.org/licenses/by/4.0/

----------------------------------
The code is tested on Mac Os X.

----------------------------------

To compile:

cd Experiment\ I/LabExperiment1/program/experiment/jcUtil
  
  make
  
  cd ../
  
  make
  
To run:

./main 1 1 
  
 you may also try ./main 2 2 or any combinations below of ./main <parameter1> <parameter2>
 
 participantID parameter1 parameter2 
  
  1          1          1               
  
  2          2          1               
  
  3          3          1               
  
  4          4          1               
  
  5          5          1               
  
  6          1          2               
  
  7          2          2               
  
  8          3          2               
  
  9          4          2               
  
  10         5          2               
 
where parameter2 is the block ID (the row index in a latin square)
parameter1 is the index in the current block.


Similarily, one may compile the training program by doing the following:

To compile:
  
  cd Experiment\ I/LabExperiment1/program/training/jcUtil
  
  make 
  
  cd ../
  
  make
 
 To run: 
  
  ./run
  
----------------------------------
To compile experiment 2, please use the same steps.

To compile:
  
  cd Experiment\ II/LabExperiment1/program/experiment/jcUtil
  
  make
  
  cd ../
  
  make
  
To run the training code:
  
  ./startTraining 1

To run the formal study code:
  
  ./startFormal 1 
   
----------------------------------

Quantum spin visualizations will appear on the screen. Left mouse drag will rotate the scene. One may also click the button to move to the next task.


  
  
