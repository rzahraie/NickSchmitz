;------------------------------------------------------------------
; Reference M5 Switch Matrix - Numeric version
; A: Nick Schmitz 2/28/95   rev 0.2 3/15/95

; This is a sample switch matrix design, probably unsuitable
; for routing real signals, but created to demonstrate nuances
; of format for definition purposes. (One Segment Defined.)

; Pattern modified to contain local feedback src in L1 listing
; L2 & 3 scrambled to minimize cross talk.
; Record type 0 - Part type header added
;------------------------------------------------------------------
; Key:
; First char =';' comment line - ignore whole line.
; All other entries strictly numerical.

; 1st # - Record type - Mux/Demux Level 1,2,3. 0=PT Header
; 2nd # - Segement # - Currently allways = 1.
; 3rd # - # of mux/demux connections.  Currently:  L1=8  or L2,3=3.
; (Dictates how many arguments will follow block & mux designators)

; Remainder of line repeated one or more times.
; 4th # - Block designator A,B,C,D = 1,2,3,4 (for Level 1 & 2), 0 for L3.
; 5th # - Mux/Demux number - 0..31, for level 1,2   - 0..127 for level 3.
; #-6..12 or #-6..8, Segment source or target line  - {0..127 Currently.}
;------------------------------------------------------------------
;  PART-N#    Blk  MC NIN Sln Sg Gln TMC I/O
0  M5-small     4  4  8   16   4  16  64   32
;
; Level 1 Array Inputs - 3:1 Mux 18.3
; 00 01 02 00 01 02
1 1 3  1 00 00 06 11  2 00 01 07 12
1 1 3  1 01 01 07 12  2 01 02 08 13
1 1 3  1 02 02 08 13  2 02 03 09 14
1 1 3  1 03 03 09 14  2 03 04 10 15
1 1 3  1 04 04 10 15  2 04 05 11 11
1 1 3  1 05 05 11 14  2 05 00 06 15
1 1 3  1 06 04 10 13  2 06 05 11 14
1 1 3  1 07 03 09 12  2 07 04 10 13
1 1 3  1 08 02 08 11  2 08 03 09 12
1 1 3  1 09 01 07 14  2 09 02 08 15
;
; 00 01 02 00 01 02
1 1 3  3 00 05 11 15  4 00 06 12 16 
1 1 3  3 01 04 10 14  4 01 05 11 15 
1 1 3  3 02 03 09 13  4 02 04 10 14 
1 1 3  3 03 02 08 12  4 03 03 09 13 
1 1 3  3 04 01 07 11  4 04 02 08 12 
1 1 3  3 05 00 06 12  4 05 01 07 13 
1 1 3  3 06 01 07 13  4 06 02 08 14 
1 1 3  3 07 02 08 14  4 07 03 09 15 
1 1 3  3 08 03 09 15  4 08 04 10 16 
1 1 3  3 09 04 10 12  4 09 05 11 13 
;
; Level 2 Sources 3:1 Demux 42.7 
; 01 02 003 01 02 003 01 02 003 01 02 003
2 1 3  1 00 00 06 11  2 00 01 06 11  3 00 02 06 11  4 00 03 06 11
2 1 3  1 01 03 09 14  2 01 04 09 14  3 01 05 09 14  4 01 06 09 14
2 1 3  1 02 01 07 12  2 02 02 07 12  3 02 03 07 12  4 02 04 07 12
2 1 3  1 03 04 10 15  2 03 05 10 15  3 03 06 10 15  4 03 07 10 15 
;
; Level 3 Sources 3:1 Demux 043 
; 01 02 003 01 02 003 01 02 003 01 02 003 
3 1 3  0 00 01 06 11  0 04 00 06 11  0 08 03 06 11  0 12 02 06 11
3 1 3  0 01 04 09 14  0 05 03 09 14  0 09 06 09 14  0 13 05 09 14
3 1 3  0 02 02 07 12  0 06 01 07 12  0 10 04 07 12  0 14 03 07 12
3 1 3  0 03 05 10 15  0 07 04 10 15  0 11 07 10 15  0 15 06 10 15
; eof
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  




  
  
