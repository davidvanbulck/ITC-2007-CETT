#!/bin/bash

files=("set1.exam" "set2.exam" "set3.exam" "set4.exam" "set5.exam" "set6.exam" "set7.exam" "set8.exam" "set9.exam" "set10.exam" "set11.exam" "set12.exam")
initMethods=("SD" "OBSI")

for init in "${initMethods[@]}"; do
	for file in "${files[@]}"; do
	    base_name="${file%.*}"  # Extract the part before the extension
	    extension="${file##*.}"  # Extract the extension

	    for ((j=0; j<=99; j++)); do
		    for ((i=1; i<=10; i++)); do
			sol_file="${base_name}_${init}_${j}.sol"
			new_file="${base_name}_${i}_${init}_${j}.sol"
			cp "$sol_file" "$new_file"
			echo "Created $new_file from $sol_file"
		    done
	    done
	done
done

