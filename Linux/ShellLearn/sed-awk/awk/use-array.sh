#!/bin/sh

# use awk array 

awk '
BEGIN{
    array["a"] = "aaa"
    array["b"] = "bbb"
    # how to find item in array
    if ("c" in array){
        print "found c"
    }
    else{
        print "not found c"
    }
}

{
    if ($0 in array_2){
        print "repeat" $0
    }
    else{
        array_2[$0] = $0
    }
}


' $*

