The eagle subdirectory contains two sample boards, created with Cadsoft Eagle.

The boards were then exported from Eagle to HyperLynx format using hyperlynx.ulp, a script in Eagle User Language.

The hyperlynx.ulp script can be downloaded from http://www.cadsoftusa.com/downloads/file/hyperlynx_5_7.zip

To export a bord in hyperlynx format, in Eagle first run "ratsnest" then "run hyperlynx".

Changes wrt version 4.14: 

- remove spurious 'f' in line 1154

- Use layer names throughout, instead of a mix of layer names and layer numbers.  

- Put strings between quotes, eg. device and layer names.

- Add export of Polygons

koen 
20/10/12
