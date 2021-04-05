#!/bin/bash

input="test_vectors.txt"

i=0
k=1
msg=""
md=""
length=0

while IFS= read -r line
do
    dir="test$k"

    if [ `expr $i % 4` == 0 ]; then
        rm -rf $dir
        mkdir $dir
    fi


    if [ `expr $i % 4` == 0 ]; then
        j=0

        for word in $line
        do
            if [ `expr $j` == 2 ]; then
                length=`echo $word | sed 's/[^0-9]*//g'`
                ((length=length/8))
                echo "$length" >> "$dir/summary.txt"
            else
                echo -n "$word " >> "$dir/summary.txt"
            fi

            ((j=j+1))
        done
    elif [ `expr $i % 4` == 1 ]; then
        echo $line >> "$dir/summary.txt"

        j=0

        for word in $line
        do
            if [ `expr $j` == 2 ]; then
                msg="$word"
            fi

            ((j=j+1))
        done
    elif [ `expr $i % 4` == 2 ]; then
        echo $line >> "$dir/summary.txt"

        j=0

        for word in $line
        do
            if [ `expr $j` == 2 ]; then
                md="$word"
            fi

            ((j=j+1))
        done

        perl -i -pe 's/\r//' "$dir/summary.txt"

        msgbytes=$(echo -ne $msg | sed -e 's/../\\x&/g')
        echo -ne $msgbytes > "$dir/msg.txt"

        mdbytes=$(echo -ne $md | sed -e 's/../\\x&/g')
        echo -ne $mdbytes > "$dir/md.txt"

        o="$(../sha3-test -v -l $length -i $dir/msg.txt -o $dir/md_out.txt 2>&1)"
        echo "$o" > "$dir/output.txt"
        diff=$(diff $dir/md.txt $dir/md_out.txt)

        if [ "$diff" != "" ]; then
            echo "Test $k: failed"
        else
            echo "Test $k: passed"
        fi

        ((k=k+1))
    fi

    ((i=i+1))
done < "$input"
