1:@:(dbr bind Debug)@:(9!:19)2^_44[(echo^:ECHOFILENAME) './g011i.ijs'
NB. <./ B ---------------------------------------------------------------

0 0 0 1 -: <./ 0 0 1 1 ,: 0 1 0 1

min=: 4 : 'x<.y'

(<./"1 -: min/"1) x=:?3 5 17$2
(<./"2 -: min/"2) x
(<./"3 -: min/"3) x

(<./"1 -: min/"1) x=:?3 5 32$2
(<./"2 -: min/"2) x
(<./"3 -: min/"3) x

(<./"1 -: min/"1) x=:?3 8 32$2
(<./"2 -: min/"2) x
(<./"3 -: min/"3) x

f=: 3 : '(<./ -: min/) y?@$2'
,f"1 x=:7 8 9,."0 1 [ _1 0 1+  255
,f"1 |."1 x
,f"1 x=:7 8 9,."0 1 [ _1 0 1+4*255
,f"1 |."1 x


NB. <./ I ----------------------------------------------------------------

min=: 4 : 'x<.y'

(<./ -: min/) x=:1 2 3 1e9 2e9
(<./ -: min/) |.x

(<./   -: min/  ) x=:_1e4+?    23$2e4
(<./   -: min/  ) x=:_1e4+?4   23$2e4
(<./"1 -: min/"1) x
(<./   -: min/  ) x=:_1e4+?7 5 23$2e4
(<./"1 -: min/"1) x
(<./"2 -: min/"2) x

(<./   -: min/  ) x=:_1e9+?    23$2e9
(<./   -: min/  ) x=:_1e9+?4   23$2e9
(<./"1 -: min/"1) x
(<./   -: min/  ) x=:_1e9+?7 5 23$2e9
(<./"1 -: min/"1) x
(<./"2 -: min/"2) x


NB. <./ D ----------------------------------------------------------------

min=: 4 : 'x<.y'

(<./   -: min/  ) x=:0.01*_1e4+?    23$2e4
(<./   -: min/  ) x=:0.01*_1e4+?4   23$2e4
(<./"1 -: min/"1) x
(<./   -: min/  ) x=:0.01*_1e4+?7 5 23$2e4
(<./"1 -: min/"1) x
(<./"2 -: min/"2) x
(<./   -: min/  )@> x=:(0.01*_1e9+2e9 ?@$~ ])&.> 4 ,&.>/ >: i. 100
(<./   -: min/  )@> x=:(0.01*_1e9+2e9 ?@$~ ])&.> >: i. 100


NB. <./ Z ----------------------------------------------------------------

min=: 4 : 'x<.y'

(<./   -: min/  ) x=:(0j1-0j1)+0.1*_1e2+?    23$2e2
(<./   -: min/  ) x=:(0j1-0j1)+0.1*_1e2+?4   23$2e2
(<./"1 -: min/"1) x
(<./   -: min/  ) x=:(0j1-0j1)+0.1*_1e2+?7 5 23$2e2
(<./"1 -: min/"1) x
(<./"2 -: min/"2) x

4!:55 ;:'f min x'


