# t9search
First "introduction to programming systems (IZP) in FIT VUT" university project T9 search in dataset with contacts names and mobile phones.

Run program - ./t9search {sequence of numbers in prone number or sequence of numbers in letter representation on mobile phone keyboard} <{name of file}

for example:

$ ./t9search <seznam.txt

petr dvorak, 603123456

jana novotna, 777987654

bedrich smetana ml., 541141120



$ ./t9search 12 <seznam.txt

petr dvorak, 603123456

bedrich smetana ml., 541141120



$ ./t9search 686 <seznam.txt
jana nOVOtna, 777987654



$ ./t9search 38 <seznam.txt

pETr DVorak, 603123456

bedrich smETana ml., 541141120



$ ./t9search 111 <seznam.txt

Not found
