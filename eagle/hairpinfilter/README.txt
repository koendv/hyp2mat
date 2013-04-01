This directory is a demo of a hairpin filter. 
It contains the following files:

demo_hairpin.m                     matlab simulation script
hairpinfilter.sch                  CADsoft Eagle schematic
hairpinfilter.brd                  CADsoft Eagle board
hairpinfilter.HYP                  board exported to hyperlynx
hairpinfilter_board.png            screenshot of board after import in CSXCAD
hairpinfilter_simulation.png       openEMS simulation output
README.txt                         this file

To run the demo, first install and configure matlab or octave, CSXCAD, openEMS and hyp2mat. 
Copy the demo files to a directory where you have read and write access.
Start up matlab or octave from that directory, and type 'demo_hairpin'.
When the AppCSXCAD window appears, exit AppCSXCAD. 
After a few minutes simulation output should appear.

On Linux:

cd
mkdir demo
cp /usr/share/hyp2mat/eagle/hairpinfilter/* demo/
cd demo
octave
demo_hairpin

