#!/bin/bash
echo "Start Test"
#./test -al | tee test1.txt

#append=$(date +"%y_%m_%d")
#mkdir ./data/$append/
start=$(date +"%m_%d_%H_%M")
for (( c=1; c<=48; c++ ))
do
	
	echo "Test number:" $c >> ./logs/tune$start.log

	./tune | while IFS= read -r line; do echo "$(date) $line"; done | tee -a ./logs/tune$start.log

	append=$(date +"%m/%d_%H:%M:%S")
	
	
	
	FIRSTLINE=$(sed -n 1p fData.txt)
	SECONDLINE=$(sed -n 2p fData.txt)
	THIRDLINE=$(sed -n 3p fData.txt)

	echo -e $append '\t\t' $FIRSTLINE '\t\t' $SECONDLINE '\t\t' $THIRDLINE >> fcurrent

	
	FIRSTLINE2=$(sed -n 1p testData.txt)
	SECONDLINE2=$(sed -n 2p testData.txt)
	THIRDLINE2=$(sed -n 3p testData.txt)
	echo -e $append '\t\t' $FIRSTLINE2 '\t\t' $SECONDLINE2 '\t\t' $THIRDLINE2 >> current
	
			
	rm fData.txt
	rm testData.txt
	#awk '{x=$2*0.0005;print $1"\t\t"x}' current >> currentInch
	awk '{x+=$2*0.0005;y+=$3*.0005;z+=$4*.005;print $1"\t\t"$2"\t\t"$3"\t\t"$4"\t\t"x"\t\t"y"\t\t"z}' current > currentInch	
	gnuplot fdisplot.gp
	gnuplot fplot.gp
	gnuplot displot.gp
	rm currentInch

	#MOVE DATA TO SERVER	
	cp /home/mfors1/Desktop/PCCOrg/fcurrent /home/mfors1/server/data/Current\ Test/
	cp /home/mfors1/Desktop/PCCOrg/current /home/mfors1/server/data/Current\ Test/
	cp /home/mfors1/Desktop/PCCOrg/freqCurrent.jpeg /home/mfors1/server/data/Current\ Test/
	cp /home/mfors1/Desktop/PCCOrg/freqdisCurrent.jpeg /home/mfors1/server/data/Current\ Test/
	cp /home/mfors1/Desktop/PCCOrg/disCurrent.jpeg /home/mfors1/server/data/Current\ Test/
	

	echo "Next test in .5 hours" >> ./logs/tune$start.log
	
	#MOVE LOG TO SERVER
	cp ./logs/tune$start.log /home/mfors1/server/data/Current\ Test/
	
	
	echo "Resting"
	sleep 30m
	


done
echo "Done Test"

#mv stretchData.txt ./data/$append/stretchData.txt
#mv testconfig.txt ./data/$append/testconfig.txt
