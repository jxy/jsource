1:@:(dbr bind Debug)@:(9!:19)2^_44[(echo^:ECHOFILENAME) './gi.ijs'
NB. i.y -----------------------------------------------------------------

randuni''

iota =: 3 : '+/&>{(}.*/\.|y,1)*&.>((0>y)*|>:y)+&.>(*y)*&.>i.&.>|y'

a =: i.1+?50
1 = $$a
0 = 0{a
_1 *./ . = 2-/\a
((#a)$(0{a)+_1{a) -: a+|.a

p =: i.q=:_5+?10 10 10
($p) -: |q
p -: iota q

'domain error' -: i. etx 'abc'
'domain error' -: i. etx u:'abc'
'domain error' -: i. etx 10&u:'abc'
'domain error' -: i. etx s:@<"0 'abc'
'domain error' -: i. etx 3.4 5
'domain error' -: i. etx 3j4 5
'domain error' -: i. etx 3 4;5

mtchontally =: ((* i.~) -: (# i. ]))
mtchonreshape =: ((i.~@]) -: (,/@($ ,:) i. ]))
mtchontally0 =: ((* i.!.0~) -: (# i.!.0 ]))
mtchonreshape0 =: ((i.!.0~@]) -: (,/@($ ,:) i.!.0 ]))


NB. x i.y ---------------------------------------------------------------

NB. Boolean
a=:1=?10 5$2
a-:(i.~a){a
(i.~a)-:i.~<"_1 a
a-:(a i.0+a){a
a-:(a i.[&.o.a){a
a-:(a i.[&.(0j1&*)a){a
0=a i.0{a
(#a)=a i.4 5 6 7 8
(#a)=a i.'abcde'
(b*#a) -: (a=:(>:?20)$0) i. b=:?30$2
20 mtchontally a =: (5 + ? 100) ?@$ 2
20 mtchonreshape a =: (5 + ? 100) ?@$ 2
20 mtchontally a =: (5 + ? 20 20) ?@$ 2
20 mtchonreshape a =: (5 + ? 20 20) ?@$ 2
20 mtchontally a =: (5 + ? 10 10 10) ?@$ 2
20 mtchonreshape a =: (5 + ? 10 10 10) ?@$ 2


NB. literal
a=:a.{~32+?10 5$95
a-:(i.~a){a
(i.~a)-:i.~<"_1 a
0=a i.0{a
(#a)=a i.4 5 6 7 8
(#a)=a i. (}.$a)$_1
 
(b*#a) -: (a=:(>:?40)$'axy') i. (b=:?30$2){'ab'
(1|.a) -: (a i.1|.a){a=:a.{~?117 1$#a.
(1|.a) -: (a i.1|.a){a=:a.{~?117 2$#a.
(1|.a) -: (a i.1|.a){a=:a.{~?117 3$#a.
(1|.a) -: (a i.1|.a){a=:a.{~?117 4$#a.
(1|.a) -: (a i.1|.a){a=:a.{~?117 5$#a.
(1|.a) -: (a i.1|.a){a=:a.{~?117 6$#a.
(1|.a) -: (a i.1|.a){a=:a.{~?117 7$#a.
(1|.a) -: (a i.1|.a){a=:a.{~?117 8$#a.
(#a)=a i. (}.$a)$'a'  NB. shouldn't match
20 mtchontally a =: a. {~ (5 + ? 100) ?@$ #a.
20 mtchonreshape a =: a. {~ (5 + ? 100) ?@$ #a.
20 mtchontally a =: a. {~ (5 + ? 20 20) ?@$ #a.
20 mtchonreshape a =: a. {~ (5 + ? 20 20) ?@$ #a.
20 mtchontally a =: a. {~ (5 + ? 10 10 10) ?@$ #a.
20 mtchonreshape a =: a. {~ (5 + ? 10 10 10) ?@$ #a.

(1|.a) -: (a i. 1|.a){a=:a.{~?7000 2$#a.
(1|.a) -: (a i. 1|.a){a=:a.{~?7000 4$#a.
(1|."2 a) -: (a i."(2) 1|."2 a){"_1 a=:a.{~?7 5000 2$#a.
(1|."2 a) -: (a i."(2) 1|."2 a){"_1 a=:a.{~?7 5000 4$#a.
(#a)=a i. (}.$a)$'a'

NB. literal2
a=:adot1{~32+?10 5$95
a-:(i.~a){a
(i.~a)-:i.~<"_1 a
0=a i.0{a
(#a)=a i.4 5 6 7 8
(#a)=a i. (}.$a)$_1
(b*#a) -: (a=:(>:?40)$u:'axy') i. (b=:?30$2){u:'ab'
(1|.a) -: (a i.1|.a){a=:adot1{~?117 1$#adot1
(1|.a) -: (a i.1|.a){a=:adot1{~?117 2$#adot1
(1|.a) -: (a i.1|.a){a=:adot1{~?117 3$#adot1
(1|.a) -: (a i.1|.a){a=:adot1{~?117 4$#adot1
(1|.a) -: (a i.1|.a){a=:adot1{~?117 5$#adot1
(1|.a) -: (a i.1|.a){a=:adot1{~?117 6$#adot1
(1|.a) -: (a i.1|.a){a=:adot1{~?117 7$#adot1
(1|.a) -: (a i.1|.a){a=:adot1{~?117 8$#adot1
(#a)=a i. (}.$a)$'a'  NB. shouldn't match

(1|.a) -: (a i. 1|.a){a=:adot1{~?7000 2$#adot1
(1|.a) -: (a i. 1|.a){a=:adot1{~?7000 4$#adot1
(1|."2 a) -: (a i."(2) 1|."2 a){"_1 a=:adot1{~?7 5000 2$#adot1
(1|."2 a) -: (a i."(2) 1|."2 a){"_1 a=:adot1{~?7 5000 4$#adot1
(#a)=a i. (}.$a)$'a'  NB. shouldn't match
(#a)=a i. (}.$a)$'a'  NB. shouldn't match
20 mtchontally a =: adot1 {~ (5 + ? 100) ?@$ #adot1
20 mtchonreshape a =: adot1 {~ (5 + ? 100) ?@$ #adot1
20 mtchontally a =: adot1 {~ (5 + ? 20 20) ?@$ #adot1
20 mtchonreshape a =: adot1 {~ (5 + ? 20 20) ?@$ #adot1
20 mtchontally a =: adot1 {~ (5 + ? 10 10 10) ?@$ #adot1
20 mtchonreshape a =: adot1 {~ (5 + ? 10 10 10) ?@$ #adot1

NB. literal4
a=:adot2{~32+?10 5$95
a-:(i.~a){a
(i.~a)-:i.~<"_1 a
0=a i.0{a
(#a)=a i.4 5 6 7 8
(b*#a) -: (a=:(>:?40)$10&u:'axy') i. (b=:?30$2){10&u:'ab'
(1|.a) -: (a i.1|.a){a=:adot2{~?117 1$#adot2
(1|.a) -: (a i.1|.a){a=:adot2{~?117 2$#adot2
(1|.a) -: (a i.1|.a){a=:adot2{~?117 3$#adot2
(1|.a) -: (a i.1|.a){a=:adot2{~?117 4$#adot2
(1|.a) -: (a i.1|.a){a=:adot2{~?117 5$#adot2
(1|.a) -: (a i.1|.a){a=:adot2{~?117 6$#adot2
(1|.a) -: (a i.1|.a){a=:adot2{~?117 7$#adot2
(1|.a) -: (a i.1|.a){a=:adot2{~?117 8$#adot2
(#a)=a i. (}.$a)$'a'  NB. shouldn't match

(1|.a) -: (a i. 1|.a){a=:adot2{~?7000 2$#adot2
(1|.a) -: (a i. 1|.a){a=:adot2{~?7000 4$#adot2
(1|."2 a) -: (a i."(2) 1|."2 a){"_1 a=:adot2{~?7 5000 2$#adot2
(1|."2 a) -: (a i."(2) 1|."2 a){"_1 a=:adot2{~?7 5000 4$#adot2
(#a)=a i. (}.$a)$'a'  NB. shouldn't match
20 mtchontally a =: adot2 {~ (5 + ? 100) ?@$ #adot2
20 mtchonreshape a =: adot2 {~ (5 + ? 100) ?@$ #adot2
20 mtchontally a =: adot2 {~ (5 + ? 20 20) ?@$ #adot2
20 mtchonreshape a =: adot2 {~ (5 + ? 20 20) ?@$ #adot2
20 mtchontally a =: adot2 {~ (5 + ? 10 10 10) ?@$ #adot2
20 mtchonreshape a =: adot2 {~ (5 + ? 10 10 10) ?@$ #adot2

NB. symbol
a=:sdot0{~32+?10 5$95
a-:(i.~a){a
(i.~a)-:i.~<"_1 a
0=a i.0{a
(#a)=a i.4 5 6 7 8
(#a)=a i. (}.$a)$s: 'jlisgdflgjhgf '
(b*#a) -: (a=:(>:?40)$s:@<"0 'axy') i. (b=:?30$2){s:@<"0 'ab'
(1|.a) -: (a i.1|.a){a=:sdot0{~?117 1$#sdot0
(1|.a) -: (a i.1|.a){a=:sdot0{~?117 2$#sdot0
(1|.a) -: (a i.1|.a){a=:sdot0{~?117 3$#sdot0
(1|.a) -: (a i.1|.a){a=:sdot0{~?117 4$#sdot0
(1|.a) -: (a i.1|.a){a=:sdot0{~?117 5$#sdot0
(1|.a) -: (a i.1|.a){a=:sdot0{~?117 6$#sdot0
(1|.a) -: (a i.1|.a){a=:sdot0{~?117 7$#sdot0
(1|.a) -: (a i.1|.a){a=:sdot0{~?117 8$#sdot0
(#a)=a i. (}.$a)$s: 'dlfdlgjd '  NB. shouldn't match

(1|.a) -: (a i. 1|.a){a=:sdot0{~?7000 2$#sdot0
(1|.a) -: (a i. 1|.a){a=:sdot0{~?7000 4$#sdot0
(1|."2 a) -: (a i."(2) 1|."2 a){"_1 a=:sdot0{~?7 5000 2$#sdot0
(1|."2 a) -: (a i."(2) 1|."2 a){"_1 a=:sdot0{~?7 5000 4$#sdot0
(#a)=a i. (}.$a)$s: 'dlfdlgjd '  NB. shouldn't match
20 mtchontally a =: sdot0 {~ (5 + ? 100) ?@$ #sdot0
20 mtchonreshape a =: sdot0 {~ (5 + ? 100) ?@$ #sdot0
20 mtchontally a =: sdot0 {~ (5 + ? 20 20) ?@$ #sdot0
20 mtchonreshape a =: sdot0 {~ (5 + ? 20 20) ?@$ #sdot0
20 mtchontally a =: sdot0 {~ (5 + ? 10 10 10) ?@$ #sdot0
20 mtchonreshape a =: sdot0 {~ (5 + ? 10 10 10) ?@$ #sdot0


NB. integer
a=:?10 5$100
a-:(i.~a){a
(i.~a)-:i.~<"_1 a
a-:(a i.[&.o.a){a
a-:(a i.[&.(0j1&*)a){a
0=a i.0{a
(#a)=a i.4 5 6 7 8
(#a)=a i.'abcde'
(b*#a) -: (a=:(>:?40)$49 9 123) i. (b=:?40$2){49 _49
(i.31) -: i.~2^i. 31
(i.31) -: i.~2^i._31
(30$0) -: i.~30$123456
(30$0) -: i.~30$_12345678
a -: (i.~a){a=:?4000$4000                   NB. small integers
(1000{.a) -: (a i.1000{.a){a=:?4000$4000    NB. small integers
(#a)=a i. (}.$a)$_1
a -: (i.~a){a=: _5 2147483647               NB. large integers
a -: (i.~a){a=:  2 2147483647               NB. large integers
a -: (i.~a){a=: ?4000$123456                NB. large integers
(1000{.a) -: (a i.1000{.a){a=:?4000$123456  NB. large integers
(#a)=a i. (}.$a)$_1
NB. long i. short, small-range then hashed
20 mtchontally a =: (5 + ? 100) ?@$ 50
20 mtchonreshape a =: (5 + ? 100) ?@$ 50
20 mtchontally a =: (5 + ? 20 20) ?@$ 200
20 mtchonreshape a =: (5 + ? 20 20) ?@$ 200
20 mtchontally a =: (5 + ? 10 10 10) ?@$ 200
20 mtchonreshape a =: (5 + ? 10 10 10) ?@$ 200
20 mtchontally a =: (5 + ? 100) ?@$ 1e6
20 mtchonreshape a =: (5 + ? 100) ?@$ 1e6
20 mtchontally a =: (5 + ? 20 20) ?@$ 1e6
20 mtchonreshape a =: (5 + ? 20 20) ?@$ 1e6
20 mtchontally a =: (5 + ? 10 10 10) ?@$ 1e6
20 mtchonreshape a =: (5 + ? 10 10 10) ?@$ 1e6

NB. floating point
a=:o._40+?10 5$100
a-:(i.   ~a){a
a-:(i.!.0~a){a
a-:(a i.    [&.(0j1&*)a){a
a-:(a i.!.0 [&.(0j1&*)a){a
(i.   ~a)-:i.   ~<"_1 a
(i.!.0~a)-:i.!.0~<"_1 a
0=a i.    0{a
0=a i.!.0[0{a
(#a)=a i.     4 5 6 7 8
(#a)=a i.!.0 [4 5 6 7 8
(#a)=a i.    'abcde'
(#a)=a i.!.0 'abcde'
(b*#a) -: (a=:(>:?40)$4.95 9 _1.62) i.    (b=:?70$2){4.95 1234
(#a)=a i. (}.$a)$_1
(b*#a) -: (a=:(>:?40)$4.95 9 _1.62) i.!.0 (b=:?70$2){4.95 1234
(#a)=a i. (}.$a)$_1
NB. long i. short intolerant
20 mtchontally0 a =: (5 + ? 100) ?@$ 0
20 mtchonreshape0 a =: (5 + ? 100) ?@$ 0
20 mtchontally0 a =: (5 + ? 20 20) ?@$ 0
20 mtchonreshape0 a =: (5 + ? 20 20) ?@$ 0
20 mtchontally0 a =: (5 + ? 10 10 10) ?@$ 0
20 mtchonreshape0 a =: (5 + ? 10 10 10) ?@$ 0

NB. complex
a=:r.?10 5$1000
a-:(i.    ~a){a
a-:(i.!.0 ~a){a
(i.   ~a)-:i.   ~<"_1 a
(i.!.0~a)-:i.!.0~<"_1 a
0=a i.    0{a
0=a i.!.0[0{a
(#a)=a i.    4 5 6 7 8
(#a)=a i.!.0[4 5 6 7 8
(#a)=a i.    'abcde'
(#a)=a i.!.0 'abcde'
(b*#a) -: (a=:(>:?40)$4j95 9 _1.62) i.    (b=:?30$2){4j95 1234
(#a)=a i. (}.$a)$_1
(b*#a) -: (a=:(>:?40)$4j95 9 _1.62) i.!.0 (b=:?30$2){4j95 1234
(#a)=a i. (}.$a)$_1
20 mtchontally0 a =: (5 + ? 100) (([: j./ [: ? 2 $ ])"0)@$ 0
20 mtchonreshape0 a =: (5 + ? 100) (([: j./ [: ? 2 $ ])"0)@$ 0
20 mtchontally0 a =: (5 + ? 20 20) (([: j./ [: ? 2 $ ])"0)@$ 0
20 mtchonreshape0 a =: (5 + ? 20 20) (([: j./ [: ? 2 $ ])"0)@$ 0
20 mtchontally0 a =: (5 + ? 10 10 10) (([: j./ [: ? 2 $ ])"0)@$ 0
20 mtchonreshape0 a =: (5 + ? 10 10 10) (([: j./ [: ? 2 $ ])"0)@$ 0

NB. boxed
t=:(1=?70$3)<;.1 ?70$100
a=:t{~?10 5$#t
a-:(i.   ~a){a
a-:(i.!.0~a){a
(i.   ~a)-:i.   ~<"_1 a
(i.!.0~a)-:i.!.0~<"_1 a
0=a i.    0{a
0=a i.!.0[0{a
(#a)=a i.    'Cogit'
(#a)=a i.!.0 'Cogit'
(#a)=a i.    4 5 6 7 8
(#a)=a i.!.0[4 5 6 7 8
(b*#a) -: (a=:(>:?40)$(<4;'aj95'),<'lieben') i. (b=:?50$2){(<4;'aj95'),<1234
(#a)=a i. (}.$a)$<'kjgfjldgf'
(b*#a) -: (a=:(>:?40)$(<4;u:'aj95'),<u:'lieben') i. (b=:?50$2){(<4;u:'aj95'),<1234
(b*#a) -: (a=:(>:?40)$(<4;10&u:'aj95'),<10&u:'lieben') i. (b=:?50$2){(<4;10&u:'aj95'),<1234
(b*#a) -: (a=:(>:?40)$(<4;s:@<"0 'aj95'),<s:@<"0 'lieben') i. (b=:?50$2){(<4;s:@<"0 'aj95'),<1234
((i.   ~x){x) -: x=:;:'i.~(?20$3){3 4;([&.o.3 4);[&.(0j1&*)3 4'
((i.!.0~x){x) -: x=:;:'i.~(?20$3){3 4;([&.o.3 4);[&.(0j1&*)3 4'
(20$0) -: i.   ~(?20$3){'';($0);(0$<'')
(20$0) -: i.!.0~(?20$3){'';($0);(0$<'')
(20$0) -: i.   ~(?20$3){3 4;([&.o.3 4);[&.(0j1&*)3 4
(20$0) -: i.!.0~(?20$3){3 4;([&.o.3 4);[&.(0j1&*)3 4
0 = (20 # <'abc') i. (10 # <'abc')
20 mtchontally0 a =: (a. {~ (#a.) ?@$~ ?)&.> (5 + ? 100) $ 10
20 mtchonreshape0 a =: (a. {~ (#a.) ?@$~ ?)&.> (5 + ? 100) $ 10
20 mtchontally0 a =: (a. {~ (#a.) ?@$~ ?)&.> (5 + ? 20 20) $ 10
20 mtchonreshape0 a =: (a. {~ (#a.) ?@$~ ?)&.> (5 + ? 20 20) $ 10
20 mtchontally0 a =: (a. {~ (#a.) ?@$~ ?)&.> (5 + ? 10 10 10) $ 10
20 mtchonreshape0 a =: (a. {~ (#a.) ?@$~ ?)&.> (5 + ? 10 10 10) $ 10

NB. x i.y encore --------------------------------------------------------

a =: 1=?100 4$2
j =: i.~a
j -: a i.0+a
j -: (0+a)i.a
a -: j{a
(#a)   -: a i.'abcd'
(2$#a) -: a i.2 4$2

0         -: (i.6 2 3)i.i.2 3
6         -: (i.6 2 3)i.2 3$9

($0)      -: (6 2 3$9)i.0 2 3$5
(5 0 4$0) -: (6 2 3$9)i.5 0 4 2 3$5

0         -: (6 2 0$9)i.2 0$0
(3$0)     -: (6 2 0$9)i.3 2 0$0
(3$0)     -: (6 2 0$0.5)i.3 2 0$'a'
(3$0)     -: (6 2 0$0.5)i.3 2 0$<''

(($b)$0)  -: ''i.b=:'abc'
(($b)$0)  -: ($0)i.b=:i.3 4
(($b)$0)  -: (0$<'')i.b=:+&.>i.3 4
0 0       -: (i.0 3 4)i.b=:i.2 3 4

3 3 3 3 3 -: (i.3 4   ) i. 5 4$'a'
3 3 3 3 3 -: (3 4$<'a') i. 5 4$'a'
3 3 3 3 3 -: (i.3 4   ) i. 5 4$u:'a'
3 3 3 3 3 -: (3 4$<u:'a') i. 5 4$u:'a'
3 3 3 3 3 -: (i.3 4   ) i. 5 4$10&u:'a'
3 3 3 3 3 -: (3 4$<10&u:'a') i. 5 4$10&u:'a'

test=: 3 : 0
 n=: ?y
 xx=: ?n$10>.<.n%3
 yy=: xx+2.5-2.5
 ((~.xx)-:~.yy),((~:xx)-:~:yy),((xx i. xx) -: yy i. yy)
)

test 1000
test 1000
test 1000

*./@test"0 [4 5$1000

2 2 2 -: (i.2 3) i. etx i.3 4
2 2 2 -: (i.2 3) i. etx 3 4$'a'
2 2 2 -: (i.2 3) i. etx 3 4$;:'Cogito, erogeneous'
3 3   -: (2 3 4$'x') i."2 etx 'kakistocracy' 

2     -: (i.2 3) i. etx 4
3 3   -: (2 3 4 6$'x') i."3 etx 'lieben'


NB. x i.y for strings x and y -------------------------------------------

map  =: 3 : '(i.-#y) (a.i.|.y)}256$#y'
ciof =: a.&i.@] { map@[

f =: i. -: ciof

((?3000$256){a.) f (?4 80$256){a.
((?3000$256){a.) f (? 300$256){a.

map  =: 3 : '(i.-#y) (adot1 i.|.y)}(#adot1)$#y'
ciof =: adot1&i.@] { map@[

f =: i. -: ciof

((?3000$(#adot1)){adot1) f (?4 80$(#adot1)){adot1
((?3000$(#adot1)){adot1) f (? 300$(#adot1)){adot1

map  =: 3 : '(i.-#y) (adot2 i.|.y)}(#adot2)$#y'
ciof =: adot2&i.@] { map@[

f =: i. -: ciof

((?3000$(#adot2)){adot2) f (?4 80$(#adot2)){adot2
((?3000$(#adot2)){adot2) f (? 300$(#adot2)){adot2

NB. x i.y for symbol x and y -------------------------------------------

map  =: 3 : '(i.-#y) (sdot0 i.|.y)}(#sdot0)$#y'
ciof =: sdot0&i.@] { map@[

f =: i. -: ciof

((?3000$(#sdot0)){sdot0) f (?4 80$(#sdot0)){sdot0
((?3000$(#sdot0)){sdot0) f (? 300$(#sdot0)){sdot0

NB. x i.y on boxed numerics ---------------------------------------------

0 0 -: i.   ~<"0 [ 1,1-2^_45
0 0 -: i.   ~<"0 |.1,1-2^_45
0 1 -: i.!.0~<"0 [ 1,1-2^_45
0 1 -: i.!.0~<"0 |.1,1-2^_45

(i.~t) -: (2*#x)$i.~x [ t=:(<"0 x), <"0          x=:?180$90
(i.~t) -: (2*#x)$i.~x [ t=:(<"0 x), <"0 (o.1)%~o.x=:?180$90
(i.~t) -: (2*#x)$i.~x [ t=:(<"0 x),~<"0 (o.1)%~o.x=:?180$90

(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.0,  x=:?40$2
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.345,x=:?40$2
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.3.5,x=:?40$2
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.3j5,x=:?40$2
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.0,  x=:?40$2e9
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.345,x=:?40$2e9
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.3.5,x=:?40$2e9
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.3j5,x=:?40$2e9
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.0,  x=:o.?40$2e7
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.345,x=:o.?40$2e7
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.3.5,x=:o.?40$2e7
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.3j5,x=:o.?40$2e7
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.0,  x=:j./?2 40$2e7
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.345,x=:j./?2 40$2e7
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.3.5,x=:j./?2 40$2e7
(2$<i.~x) -: (y i.<"0 x); (<"0 x)i.y=:<"0 }.3j5,x=:j./?2 40$2e7


NB. x i."r y ------------------------------------------------------------

g =: 4 : 'x i. y'

(i.3 0) (g"1 -: i."1) i.3 2
(i.3 0) (g"1 -: i."1) i.3 0
(i.0 0) (g"1 -: i."1) i.0 7
(i.0 7) (g"1 -: i."1) i.0 3
(i.3)   (g"1 -: i."1) i.0 7
(i.3)   (g"1 -: i."1) i.0 0
''      (g"1 -: i."1) i.0 7
''      (g"1 -: i."1) i.0 5
''      (g"1 -: i."1) i.0 0
(i.3 5) (g"1 -: i."1) 3 7$'a'
(i.3 5) (g"1 -: i."1) 3 7$<5
'abc'   (g"1 -: i."1) 7 5$3
'abc'   (g"1 -: i."1) 7 5$<3

(i.6)       -: x i."1 0 (<0 1)|:x=:a.{~6 16$32+96?96
(15-i.6)    -: x i."1 0 (<0 1)|:|."1 x
(6$0)       -: x i."1 0 {."1 x
(6$15)      -: x i."1 0 {:"1 x
(($x)$i.16) -: x i."1 x
(x=:0=5|?20 19$2) (g"1 -: i."1) 1

NB. literal
x=:a.{~?(117 7,c)$#a. [ c=:3
x (g"2  -: i."2 ) y=:a.{~?(14,   c)$#a. 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:a.{~?(117 3,c)$#a.
x (g"_1 -: i."_1) y=:a.{~?(117,  c)$#a.
x=:a.{~?(117 7,c)$#a. [ c=:4
x (g"2  -: i."2 ) y=:a.{~?(14,   c)$#a. 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:a.{~?(117 3,c)$#a.
x (g"_1 -: i."_1) y=:a.{~?(117,  c)$#a.
x=:a.{~?(117 7,c)$#a. [ c=:11
x (g"2  -: i."2 ) y=:a.{~?(14,   c)$#a. 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:a.{~?(117 3,c)$#a.
x (g"_1 -: i."_1) y=:a.{~?(117,  c)$#a.

NB. literal2
x=:adot1{~?(117 7,c)$#adot1 [ c=:3
x (g"2  -: i."2 ) y=:adot1{~?(14,   c)$#adot1 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:adot1{~?(117 3,c)$#adot1
x (g"_1 -: i."_1) y=:adot1{~?(117,  c)$#adot1
x=:adot1{~?(117 7,c)$#adot1 [ c=:4
x (g"2  -: i."2 ) y=:adot1{~?(14,   c)$#adot1 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:adot1{~?(117 3,c)$#adot1
x (g"_1 -: i."_1) y=:adot1{~?(117,  c)$#adot1
x=:adot1{~?(117 7,c)$#adot1 [ c=:11
x (g"2  -: i."2 ) y=:adot1{~?(14,   c)$#adot1 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:adot1{~?(117 3,c)$#adot1
x (g"_1 -: i."_1) y=:adot1{~?(117,  c)$#adot1

NB. literal4
x=:adot2{~?(117 7,c)$#adot2 [ c=:3
x (g"2  -: i."2 ) y=:adot2{~?(14,   c)$#adot2 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:adot2{~?(117 3,c)$#adot2
x (g"_1 -: i."_1) y=:adot2{~?(117,  c)$#adot2
x=:adot2{~?(117 7,c)$#adot2 [ c=:4
x (g"2  -: i."2 ) y=:adot2{~?(14,   c)$#adot2 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:adot2{~?(117 3,c)$#adot2
x (g"_1 -: i."_1) y=:adot2{~?(117,  c)$#adot2
x=:adot2{~?(117 7,c)$#adot2 [ c=:11
x (g"2  -: i."2 ) y=:adot2{~?(14,   c)$#adot2 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:adot2{~?(117 3,c)$#adot2
x (g"_1 -: i."_1) y=:adot2{~?(117,  c)$#adot2

NB. symbol
x=:sdot0{~?(117 7,c)$#sdot0 [ c=:3
x (g"2  -: i."2 ) y=:sdot0{~?(14,   c)$#sdot0 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:sdot0{~?(117 3,c)$#sdot0
x (g"_1 -: i."_1) y=:sdot0{~?(117,  c)$#sdot0
x=:sdot0{~?(117 7,c)$#sdot0 [ c=:4
x (g"2  -: i."2 ) y=:sdot0{~?(14,   c)$#sdot0 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:sdot0{~?(117 3,c)$#sdot0
x (g"_1 -: i."_1) y=:sdot0{~?(117,  c)$#sdot0
x=:sdot0{~?(117 7,c)$#sdot0 [ c=:11
x (g"2  -: i."2 ) y=:sdot0{~?(14,   c)$#sdot0 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:sdot0{~?(117 3,c)$#sdot0
x (g"_1 -: i."_1) y=:sdot0{~?(117,  c)$#sdot0

x=:p+?117 7$q [ p=:0 [ q=:14
x (g"1  -: i."1 ) y=:p+?q
x (g"1  -: i."1 ) x
x (g"1  -: i."1 ) y=:p+?117 3$q
x (g"1  -: i."1 ) y=:p+?12$q
x (g"_1 -: i."_1) y=:p+?117$q
x=:p+?117 7$q [ p=:_7 [ q=:14
x (g"1  -: i."1 ) y=:p+?q
x (g"1  -: i."1 ) x
x (g"1  -: i."1 ) y=:p+?117 3$q
x (g"1  -: i."1 ) y=:p+?12$q
x (g"_1 -: i."_1) y=:p+?117$q
x=:p+?117 7$q [ p=:_2000 [ q=:14
x (g"1  -: i."1 ) y=:p+?q
x (g"1  -: i."1 ) x
x (g"1  -: i."1 ) y=:p+?117 3$q
x (g"1  -: i."1 ) y=:p+?12$q
x (g"_1 -: i."_1) y=:p+?117$q
x=:p+?117 7$q [ p=:0 [ q=:1e4
x (g"1  -: i."1 ) y=:p+?q
x (g"1  -: i."1 ) x
x (g"1  -: i."1 ) y=:p+?117 3$q
x (g"1  -: i."1 ) y=:p+?12$q
x (g"_1 -: i."_1) y=:p+?117$q
x=:p+?117 7$q [ p=:_5e5 [ q=:1e6
x (g"1  -: i."1 ) y=:p+?q
x (g"1  -: i."1 ) x
x (g"1  -: i."1 ) y=:p+?117 3$q
x (g"1  -: i."1 ) y=:p+?12$q
x (g"_1 -: i."_1) y=:p+?117$q

x=:?7 63 3$q=:4
x (g"2  -: i."2 ) y=:((?10$63){0{x),?14 3$q 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:(?~1{$y){"2 y=:x,"2?7 5 3$q
x (g"_1 -: i."_1) y=:((?5$63){1{x),?2 3$q

x=:o.?7 13 3$q=:3
x (g"2  -: i."2 ) y=:((?10$13){0{x),o.?14 3$q 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:(?~1{$y){"2 y=:x,"2 o.?7 5 3$q
x (g"_1 -: i."_1) y=:((?5$13){1{x),o.?2 3$q

x=:r.?7 13 3$q=:3
x (g"2  -: i."2 ) y=:((?10$13){0{x),r.?14 3$q 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:(?~1{$y){"2 y=:x,"2 r.?7 5 3$q
x (g"_1 -: i."_1) y=:((?5$13){1{x),r.?2 3$q

x=:<"0 ?7 63 3$q=:3
x (g"2  -: i."2 ) y=:((?10$63){0{x),<"0?14 3$q 
x (g"2  -: i."2 ) x
x (g"2  -: i."2 ) y=:(?~1{$y){"2 y=:x,"2<"0?7 5 3$q
x (g"_1 -: i."_1) y=:((?5$63){1{x),<"0?2 3$q

x (g"1 2 -: i."1 2) x=:1 2,:3 4


NB. x i.!.0 "r y --------------------------------------------------------

g =: 4 : 'x i.!.0 y'

(i.3 0) (g"1 -: i.!.0"1) i.3 2
(i.3 0) (g"1 -: i.!.0"1) i.3 0
(i.0 0) (g"1 -: i.!.0"1) i.0 7
(i.0 7) (g"1 -: i.!.0"1) i.0 3
(i.3)   (g"1 -: i.!.0"1) i.0 7
(i.3)   (g"1 -: i.!.0"1) i.0 0
''      (g"1 -: i.!.0"1) i.0 7
''      (g"1 -: i.!.0"1) i.0 5
''      (g"1 -: i.!.0"1) i.0 0
(i.3 5) (g"1 -: i.!.0"1) 3 7$'a'
(i.3 5) (g"1 -: i.!.0"1) 3 7$<5
'abc'   (g"1 -: i.!.0"1) 7 5$3
'abc'   (g"1 -: i.!.0"1) 7 5$<3

(i.6)       -: x i.!.0"1 0 (<0 1)|:x=:a.{~6 16$32+96?96
(15-i.6)    -: x i.!.0"1 0 (<0 1)|:|."1 x
(6$0)       -: x i.!.0"1 0 {."1 x
(6$15)      -: x i.!.0"1 0 {:"1 x
(($x)$i.16) -: x i.!.0"1 x
(x=:0=5|?20 19$2) (g"1 -: i.!.0"1) 1

NB. literal
x=:a.{~?(117 7,c)$#a. [ c=:3
x (g"2  -: i.!.0"2 ) y=:a.{~?(14,   c)$#a. 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:a.{~?(117 3,c)$#a.
x (g"_1 -: i.!.0"_1) y=:a.{~?(117,  c)$#a.
x=:a.{~?(117 7,c)$#a. [ c=:4
x (g"2  -: i.!.0"2 ) y=:a.{~?(14,   c)$#a. 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:a.{~?(117 3,c)$#a.
x (g"_1 -: i.!.0"_1) y=:a.{~?(117,  c)$#a.
x=:a.{~?(117 7,c)$#a. [ c=:11
x (g"2  -: i.!.0"2 ) y=:a.{~?(14,   c)$#a. 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:a.{~?(117 3,c)$#a.
x (g"_1 -: i.!.0"_1) y=:a.{~?(117,  c)$#a.

NB. literal2
x=:adot1{~?(117 7,c)$#adot1 [ c=:3
x (g"2  -: i.!.0"2 ) y=:adot1{~?(14,   c)$#adot1 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:adot1{~?(117 3,c)$#adot1
x (g"_1 -: i.!.0"_1) y=:adot1{~?(117,  c)$#adot1
x=:adot1{~?(117 7,c)$#adot1 [ c=:4
x (g"2  -: i.!.0"2 ) y=:adot1{~?(14,   c)$#adot1 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:adot1{~?(117 3,c)$#adot1
x (g"_1 -: i.!.0"_1) y=:adot1{~?(117,  c)$#adot1
x=:adot1{~?(117 7,c)$#adot1 [ c=:11
x (g"2  -: i.!.0"2 ) y=:adot1{~?(14,   c)$#adot1 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:adot1{~?(117 3,c)$#adot1
x (g"_1 -: i.!.0"_1) y=:adot1{~?(117,  c)$#adot1

NB. literal4
x=:adot2{~?(117 7,c)$#adot2 [ c=:3
x (g"2  -: i.!.0"2 ) y=:adot2{~?(14,   c)$#adot2 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:adot2{~?(117 3,c)$#adot2
x (g"_1 -: i.!.0"_1) y=:adot2{~?(117,  c)$#adot2
x=:adot2{~?(117 7,c)$#adot2 [ c=:4
x (g"2  -: i.!.0"2 ) y=:adot2{~?(14,   c)$#adot2 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:adot2{~?(117 3,c)$#adot2
x (g"_1 -: i.!.0"_1) y=:adot2{~?(117,  c)$#adot2
x=:adot2{~?(117 7,c)$#adot2 [ c=:11
x (g"2  -: i.!.0"2 ) y=:adot2{~?(14,   c)$#adot2 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:adot2{~?(117 3,c)$#adot2
x (g"_1 -: i.!.0"_1) y=:adot2{~?(117,  c)$#adot2

NB. symbol
x=:sdot0{~?(117 7,c)$#sdot0 [ c=:3
x (g"2  -: i.!.0"2 ) y=:sdot0{~?(14,   c)$#sdot0 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:sdot0{~?(117 3,c)$#sdot0
x (g"_1 -: i.!.0"_1) y=:sdot0{~?(117,  c)$#sdot0
x=:sdot0{~?(117 7,c)$#sdot0 [ c=:4
x (g"2  -: i.!.0"2 ) y=:sdot0{~?(14,   c)$#sdot0 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:sdot0{~?(117 3,c)$#sdot0
x (g"_1 -: i.!.0"_1) y=:sdot0{~?(117,  c)$#sdot0
x=:sdot0{~?(117 7,c)$#sdot0 [ c=:11
x (g"2  -: i.!.0"2 ) y=:sdot0{~?(14,   c)$#sdot0 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:sdot0{~?(117 3,c)$#sdot0
x (g"_1 -: i.!.0"_1) y=:sdot0{~?(117,  c)$#sdot0

x=:p+?117 7$q [ p=:0 [ q=:14
x (g"1  -: i.!.0"1 ) y=:p+?q
x (g"1  -: i.!.0"1 ) x
x (g"1  -: i.!.0"1 ) y=:p+?117 3$q
x (g"1  -: i.!.0"1 ) y=:p+?12$q
x (g"_1 -: i.!.0"_1) y=:p+?117$q
x=:p+?117 7$q [ p=:_7 [ q=:14
x (g"1  -: i.!.0"1 ) y=:p+?q
x (g"1  -: i.!.0"1 ) x
x (g"1  -: i.!.0"1 ) y=:p+?117 3$q
x (g"1  -: i.!.0"1 ) y=:p+?12$q
x (g"_1 -: i.!.0"_1) y=:p+?117$q
x=:p+?117 7$q [ p=:_2000 [ q=:14
x (g"1  -: i.!.0"1 ) y=:p+?q
x (g"1  -: i.!.0"1 ) x
x (g"1  -: i.!.0"1 ) y=:p+?117 3$q
x (g"1  -: i.!.0"1 ) y=:p+?12$q
x (g"_1 -: i.!.0"_1) y=:p+?117$q
x=:p+?117 7$q [ p=:0 [ q=:1e4
x (g"1  -: i.!.0"1 ) y=:p+?q
x (g"1  -: i.!.0"1 ) x
x (g"1  -: i.!.0"1 ) y=:p+?117 3$q
x (g"1  -: i.!.0"1 ) y=:p+?12$q
x (g"_1 -: i.!.0"_1) y=:p+?117$q
x=:p+?117 7$q [ p=:_5e5 [ q=:1e6
x (g"1  -: i.!.0"1 ) y=:p+?q
x (g"1  -: i.!.0"1 ) x
x (g"1  -: i.!.0"1 ) y=:p+?117 3$q
x (g"1  -: i.!.0"1 ) y=:p+?12$q
x (g"_1 -: i.!.0"_1) y=:p+?117$q

x=:?7 63 3$q=:4
x (g"2  -: i.!.0"2 ) y=:((?10$63){0{x),?14 3$q 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:(?~1{$y){"2 y=:x,"2?7 5 3$q
x (g"_1 -: i.!.0"_1) y=:((?5$63){1{x),?2 3$q

x=:o.?7 13 3$q=:3
x (g"2  -: i.!.0"2 ) y=:((?10$13){0{x),o.?14 3$q 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:(?~1{$y){"2 y=:x,"2 o.?7 5 3$q
x (g"_1 -: i.!.0"_1) y=:((?5$13){1{x),o.?2 3$q

x=:r.?7 13 3$q=:3
x (g"2  -: i.!.0"2 ) y=:((?10$13){0{x),r.?14 3$q 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:(?~1{$y){"2 y=:x,"2 r.?7 5 3$q
x (g"_1 -: i.!.0"_1) y=:((?5$13){1{x),r.?2 3$q

x=:<"0 ?7 63 3$q=:3
x (g"2  -: i.!.0"2 ) y=:((?10$63){0{x),<"0?14 3$q 
x (g"2  -: i.!.0"2 ) x
x (g"2  -: i.!.0"2 ) y=:(?~1{$y){"2 y=:x,"2<"0?7 5 3$q
x (g"_1 -: i.!.0"_1) y=:((?5$63){1{x),<"0?2 3$q

x (g"1 2 -: i.!.0"1 2) x=:1 2,:3 4


NB. x i. y on floating point --------------------------------------------

0 0 0 0 -: i.~ 1+2^-45 46 47 48

NB. Verify ctmask is not too small
(i. 100001) -: i.~ */\ 1 , 100000 # (1 + (2^-52)) + 2^-44
(0 , i. 100000) -: i.~ */\ 1 , 100000 # (1 - (2^-52)) + 2^-44
(i. 100001) -: i.!.(2^_45)~ */\ 1 , 100000 # (1 + (2^-52)) + 2^-45
(0 , i. 100000) -: i.!.(2^_45)~ */\ 1 , 100000 # (1 - (2^-52)) + 2^-45
(i. 100001) -: i.!.(2^_46)~ */\ 1 , 100000 # (1 + (2^-52)) + 2^-46
(0 , i. 100000) -: i.!.(2^_46)~ */\ 1 , 100000 # (1 - (2^-52)) + 2^-46
(i. 100001) -: i.!.(2^_47)~ */\ 1 , 100000 # (1 + (2^-52)) + 2^-47
(0 , i. 100000) -: i.!.(2^_47)~ */\ 1 , 100000 # (1 - (2^-52)) + 2^-47
(i. 100001) -: i.!.(2^_48)~ */\ 1 , 100000 # (1 + (2^-52)) + 2^-48
(0 , i. 100000) -: i.!.(2^_48)~ */\ 1 , 100000 # (1 - (2^-52)) + 2^-48
(i. 100001) -: i.!.(2^_49)~ */\ 1 , 100000 # (1 + (2^-52)) + 2^-49
(0 , i. 100000) -: i.!.(2^_49)~ */\ 1 , 100000 # (1 - (2^-52)) + 2^-49
(i. 100001) -: i.!.(2^_50)~ */\ 1 , 100000 # (1 + (2^-52)) + 2^-50
(0 , i. 100000) -: i.!.(2^_50)~ */\ 1 , 100000 # (1 - (2^-52)) + 2^-50


f=: 4 : 0
 ct=. x
 y -: (i.!.ct~ y){y
)

(2^-34+-:i.3 10) f"0 1 x=: 0.001 * _1e5 + ?777$2e5

f1=: 3 : 0
 t -: (i.~ t){t=. y+i.1000
)

f1"0] 10^i.2 10

f2=: 3 : 0
 t -: (i.~ t){t=. y+?~1000
)

f2"0] 10^i.2 10

t=: 9!:18 ''
f=: 1: i."1~ =/~
(i. -: f)~ x=: 1+    t*i.50
(i. -: f)~ x=: 1+0.4*t*i.50
(i. -: f)~ x=: 1+0.5*t*i.50
(i. -: f)~ x=: 1+    t*?~50
(i. -: f)~ x=: 1+0.4*t*?~50
(i. -: f)~ x=: 1+0.5*t*?~50

x (i. -: f) y [ x=: 1+    t*i.50 [ y=: 1+    t*?~60
x (i. -: f)~y
x (i. -: f) y [ x=: 1+0.4*t*i.50 [ y=: 1+0.4*t*?~60
x (i. -: f)~y
x (i. -: f) y [ x=: 1+0.5*t*i.50 [ y=: 1+0.5*t*?~60
x (i. -: f)~y

NB. Check for proper selection of sequential search.
f =: 4 : 0&>   NB. returns 1 if sequential search selected.  x is shape min 2 in first axis; y is # items of y
xop =: _2 (_2}) _1 (_1)} 0 (0}) 10 * >: 1e6 ?@$~ x
yop1 =: y {. xop  NB. integer 0 finishes search too fast - it makes hash-on-w look like sequential
yopn =: >: xop {~  y ?@$ #xop  NB. randomly misses, finishes slow but perhaps not so slow if hashing
%/ 10000&(6!:2) 'xop i. yop1' ,: 'xop i. yopn'
)
THRESHOLD +.  0.5 > %/"1 (1000 2000) f/ 7 8  NB. 7 is sequential, has lower ratio = higher discrepancy
THRESHOLD +.  0.8 > %/ (10 11) f/ 1000 2000  NB. 10 is sequential, has lower ratio = higher discrepancy
THRESHOLD +.  1.5 >  1000 2000 f/ 1000 2000   NB. if sequential, the ratio will be high
NB. That got the early test, the following gets the later test
THRESHOLD +.  0.5 > %/"1 (,&2&.> 1000 2000) f/ 7 8  NB. 7 is sequential, has lower ratio = higher discrepancy
THRESHOLD +.  0.8 > %/ (,&2&.> 10 11) f/ 1000 2000  NB. 10 is sequential, has lower ratio = higher discrepancy
THRESHOLD +.  1.5 >  (,&2&.> 1000 2000) f/ 1000 2000   NB. if sequential, the ratio will be high

4!:55 ;:'a adot1 adot2 sdot0 b c ciof ct f f1 f2 g iota j map n p q t test x xx y yy'
4!:55 ;:'xop yop1 yopn'
4!:55 ;: 'mtchontally mtchonreshape mtchontally0 mtchonreshape0'
randfini''

