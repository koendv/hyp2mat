This directory is a demo of a notch filter. 
It contains the following files:

demo_notch.m                      matlab simulation script
notchfilter.sch                   CADsoft Eagle schematic
notchfilter.brd                   CADsoft Eagle board
notchfilter.HYP                   board exported to hyperlynx
notchfilter_board.png             screenshot of board after import in CSXCAD
notchfilter_simulation.png        openEMS simulation output
README.txt                        this file

To run the demo, first install and configure matlab or octave, CSXCAD, openEMS and hyp2mat. 
Copy the demo files to a directory where you have read and write access.
Start up matlab or octave from that directory, and type 'demo_notch'.
When the AppCSXCAD window appears, exit AppCSXCAD.
After a few minutes simulation output should appear.

On Linux:

cd
mkdir demo
cp /usr/share/hyp2mat/eagle/notchfilter/* demo/
cd demo
octave
demo_notch

