echo "Assignment 1-b\n
Write a program to implement an address book with options given below:\n 
a) Create address book.\n
b) View address book.\n
c) Insert a record.\n
d) Delete a record.\n
e) Modify a record.\n
f) Exit\n\n"

opt=1
# read  -p "Enter File Name: "


while [ "$opt" -lt 6 ]
do


	echo "Choose one of the Following\n1. Create a New Address Book\n2. View Records\n3. Insert new Record\n4. Delete a Record\n5. Modify a Record\n6. Exit"
	# echo -e, enables special features of echo to use \n \t \b etc.
	read opt
	case $opt in

	1)
		echo "Enter filename"
		read fileName
		if [ -e $fileName ] ; then	# -e to check if file exists, if exits remove the file
			rm $fileName
		fi
		cont=1
		echo  "NAME\tNUMBER\t\tADDRESS\n===============================\n" | cat >> $fileName
		while [ "$cont" -gt 0 ]
		do
			echo "Enter Name"
			read name
			echo "Enter Phone Number of $name"
			read number
			echo "Enter Address of $name"
			read address
			echo "$name\t$number\t\t$address" | cat >> $fileName
			echo "Enter 0 to Stop, 1 to Enter next"
			read cont
		done
		;;

	2)
		
		cat $fileName
		;;
	3)
		
		echo "Enter Name"
		read name
		echo "Enter Phone Number of $name"
		read number
		echo "Enter Address of $name"
		read address
		echo "$name\t$number\t\t$address" | cat >> $fileName
		;;
	4)
		
		echo "Delete record\nEnter Name/Phone Number"
		read pattern
		temp="temp"
		grep -v $pattern $fileName | cat >> $temp 
		rm $fileName
		cat $temp | cat >> $fileName
		rm $temp
		;;
	5)
		
		echo "Modify record\nEnter Name/Phone Number"
		read pattern
		temp="temp"
		grep -v $pattern $fileName | cat >> $temp
		rm $fileName
		cat $temp | cat >> $fileName
		rm $temp
		echo "Enter Name"
		read name
		echo "Enter Phone Number of $name"
		read number
		echo "Enter Address of $name"
		read address
		echo "$name\t$number\t\t$address" | cat >> $fileName
		;;
	esac
done
