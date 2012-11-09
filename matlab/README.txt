hyp2mat translates each record and subrecord from the Hyperlynx file to a Matlab function call.

Each function is called with two parameters: CSX and s.
The CSX struct contains the model geometry; the s struct contains Hyperlynx record parameters.

The s struct always contains at least the following field:

s.lineno                  line number of record in .hyp file

This allows error messages to print the line number in the hyp file where the error occurs.

During processing, the following fields are added to the CSX struct:
These fields, with the exception of pin_list, are removed from the CSX struct after processing if CSX.debug is false.

CSX.all_nets              list of all net names
CSX.arc_segments          number of linear segments per arc
CSX.board_outline         series of coordinates which give the outline of the pcb. 
CSX.conductivity          conductivity of copper layers
CSX.current_net           name of the net which is currently being parsed
CSX.debug                 true if debug info is to be printed
CSX.draw_board            flag; set if the board still needs to be set up.
CSX.padstack              list of padstacks ( PADSTACK record )
CSX.pin_list              list of port names ( PIN record)
CSX.plane_sep             default plane separation ( PLANE_SEP record)
CSX.poly_list             list of polygon id's and parameters
CSX.poly                  parameters of current polygon ( POLYGON, POLYVOID and POLYLINE record)
CSX.prio_cutout           priority of voids in copper
CSX.prio_dielectric       priority of FR4 dielectric
CSX.prio_material         priority of copper 
CSX.prio_plane            priority of power and ground planes
CSX.prio_via              priority of via metal
CSX.simulated_nets        list of nets to import in CSX, empty for import all nets.
CSX.stackup               list of pcb layers ( STACKUP record )
CSX.substrate_epr         dielectric constant of pcb board
CSX.unnamed_layer         number of unnamed layers (signal, plane or dielectric)
CSX.units                 length in meters of pcb length units ( UNITS record )
CSX.use_die_for_metal     true if metal layers have same dielectric constant and loss tangent as dielectric
CSX.zscale                length in pcb length units of layer/board thickness

The following matlab functions are provided:

ImportHyperLynx.m         load Hyperlynx file into CSX .
AddHyperLynxPort.m        Add openEMS port to HyperLynx pin.
