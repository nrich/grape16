10 SCREENWIDTH=319; SCREENHEIGHT=239; MAPWIDTH=24; MAPHEIGHT=24; MOVESPEED=0.1; ROTSPEED=0.1
20 POSX=22.0; POSY=12.0
30 DIRX=-1.0; DIRY=0.0
40 PLANEX=0.0; PLANEY=0.66
50 TIME=0.0; OLDTIME=0.0
60 DIM WORLDMAP(23,23)
70 FOR MY=0 TO 23
80 FOR MX=0 TO 23
90 READ WORLDMAP(MY,MX)
100 NEXT
110 NEXT 
120 WHILE 1
130 CLS; LINE(0,0)-(319,119),117,BF; LINE(0,120)-(319,239),94,BF
140 FOR X=0 TO SCREENWIDTH STEP 2
150 CAMERAX = 2.0 * X / SCREENWIDTH - 1.0
160 RAYDIRX = DIRX + PLANEX * CAMERAX
170 RAYDIRY = DIRY + PLANEY * CAMERAX
180 MAPX=CINT(POSX)
190 MAPY=CINT(POSY)
200 DELTADISTX=ABS(1.0/RAYDIRX)
210 DELTADISTY=ABS(1.0/RAYDIRY)
220 PERPWALLDIST = 1.0
230 STEPX=0
240 STEPY=0
250 HIT=0
260 SIDE=0
270 IF RAYDIRX<0.0 THEN STEPX=-1; SIDEDISTX = (POSX - MAPX) * DELTADISTX ELSE STEPX = 1; SIDEDISTX = (MAPX + 1.0 - POSX) * DELTADISTX
280 IF RAYDIRY<0.0 THEN STEPY=-1; SIDEDISTY = (POSY - MAPY) * DELTADISTY ELSE STEPY = 1; SIDEDISTY = (MAPY + 1.0 - POSY) * DELTADISTY
290 WHILE HIT = 0
300 IF SIDEDISTX < SIDEDISTY THEN SIDEDISTX = SIDEDISTX + DELTADISTX; MAPX = MAPX + STEPX; SIDE=0 ELSE SIDEDISTY = SIDEDISTY + DELTADISTY; MAPY = MAPY + STEPY; SIDE=1
310 IF WORLDMAP(MAPY, MAPX) > 0 THEN HIT=1
320 WEND
330 IF SIDE = 0 THEN PERPWALLDIST = (MAPX - POSX + (1.0 - STEPX) / 2.0) / RAYDIRX ELSE PERPWALLDIST = (MAPY - POSY + (1.0 - STEPY) / 2.0) / RAYDIRY
340 LINEHEIGHT = CINT(SCREENHEIGHT / PERPWALLDIST)
350 DRAWSTART = -LINEHEIGHT \ 2 + SCREENHEIGHT \ 2
360 IF DRAWSTART < 0 THEN DRAWSTART = 0
370 DRAWEND = LINEHEIGHT \ 2 + SCREENHEIGHT \ 2
380 IF DRAWEND >= SCREENHEIGHT THEN DRAWEND = SCREENHEIGHT - 1
390 TILE = WORLDMAP(MAPY, MAPX); COLOUR = 15
400 IF TILE = 1 THEN IF SIDE=0 THEN COLOUR = 124 ELSE COLOUR = 1
410 IF TILE = 2 THEN IF SIDE=0 THEN COLOUR = 28 ELSE COLOUR = 22
420 IF TILE = 3 THEN IF SIDE=0 THEN COLOUR = 19 ELSE COLOUR = 17
430 IF TILE = 4 THEN IF SIDE=0 THEN COLOUR = 7 ELSE COLOUR = 8
440 IF TILE = 0 THEN COLOUR = 0
460 LINE(X,DRAWSTART)-(X,DRAWEND),COLOUR
470 LINE(X+1,DRAWSTART)-(X+1,DRAWEND),COLOUR
480 NEXT
490 KEYPRESS=INKEY()
500 IF INKEY(119) THEN GOSUB 800
510 IF INKEY(115) THEN GOSUB 830
520 IF INKEY(97) THEN GOSUB 860
530 IF INKEY(100) THEN GOSUB 930
540 IF KEYPRESS = 49 THEN PALETTE 0
550 IF KEYPRESS = 50 THEN PALETTE 1
560 IF KEYPRESS = 51 THEN PALETTE 2
570 WAIT
700 WEND
800 IF WORLDMAP(CINT(POSY), CINT(POSX + DIRX * MOVESPEED)) = 0 THEN POSX = POSX + DIRX * MOVESPEED
810 IF WORLDMAP(CINT(POSY + DIRY * MOVESPEED), CINT(POSX)) = 0 THEN POSY = POSY + DIRY * MOVESPEED
820 RETURN
830 IF WORLDMAP(CINT(POSY), CINT(POSX - DIRX * MOVESPEED)) = 0 THEN POSX = POSX - DIRX * MOVESPEED
840 IF WORLDMAP(CINT(POSY - DIRY * MOVESPEED), CINT(POSX)) = 0 THEN POSY = POSY - DIRY * MOVESPEED
850 RETURN
860 OLDDIRX = DIRX
870 DIRX = DIRX * COS(ROTSPEED) - DIRY * SIN(ROTSPEED)
880 DIRY = OLDDIRX * SIN(ROTSPEED) + DIRY * COS(ROTSPEED)
890 OLDPLANEX = PLANEX
900 PLANEX = PLANEX * COS(ROTSPEED) - PLANEY * SIN(ROTSPEED)
910 PLANEY = OLDPLANEX * SIN(ROTSPEED) + PLANEY * COS(ROTSPEED)
920 RETURN
930 OLDDIRX = DIRX
940 DIRX = DIRX * COS(-ROTSPEED) - DIRY * SIN(-ROTSPEED)
950 DIRY = OLDDIRX * SIN(-ROTSPEED) + DIRY * COS(-ROTSPEED)
960 OLDPLANEX = PLANEX
970 PLANEX = PLANEX * COS(-ROTSPEED) - PLANEY * SIN(-ROTSPEED)
980 PLANEY = OLDPLANEX * SIN(-ROTSPEED) + PLANEY * COS(-ROTSPEED)
990 RETURN
1000 DATA 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
1010 DATA 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1020 DATA 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1030 DATA 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1040 DATA 1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1
1050 DATA 1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1
1060 DATA 1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1
1070 DATA 1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1
1080 DATA 1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1
1090 DATA 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1100 DATA 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1110 DATA 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1120 DATA 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1130 DATA 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1140 DATA 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1150 DATA 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1160 DATA 1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1170 DATA 1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1180 DATA 1,4,0,0,0,0,4,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1190 DATA 1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1200 DATA 1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1210 DATA 1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1220 DATA 1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
1230 DATA 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
