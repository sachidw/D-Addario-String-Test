#!/bin/bash
echo "Start Test"
#./test -al | tee test1.txt

#append=$(date +"%y_%m_%d")
#mkdir ./data/$append/
touch ./data/currentFrequency.dat
touch ./data/currentDisplacement.dat

start=$(date +"%m_%d_%H_%M")
for (( c=1; c<=48; c++ ))
do
	
	echo "Test number:" $c >> ./logs/tune$start.log

	#./bin/tune | while IFS= read -r line; do echo "$(date) $line"; done | tee -a ./logs/tune$start.log

	append=$(date +"%m/%d_%H:%M:%S")
	
	
	FIRSTLINE=$(sed -n 1p initialFreq.dat)
	SECONDLINE=$(sed -n 2p initialFreq.dat)
	THIRDLINE=$(sed -n 3p initialFreq.dat)

	echo -e $append '\t\t' $FIRSTLINE '\t\t' $SECONDLINE '\t\t' $THIRDLINE >> ./data/currentFrequency.dat

	
	FIRSTLINE2=$(sed -n 1p elapsedTension.dat)
	SECONDLINE2=$(sed -n 2p elapsedTension.dat)
	THIRDLINE2=$(sed -n 3p elapsedTension.dat)
	echo -e $append '\t\t' $FIRSTLINE2 '\t\t' $SECONDLINE2 '\t\t' $THIRDLINE2 >> ./data/currentDisplacement.dat
	
			
	rm initialFreq.dat
	rm elapsedTension.dat
	#awk '{x=$2*0.0005;print $1"\t\t"x}' current >> currentInch
	awk '{x+=$2*0.0005;y+=$3*.0005;z+=$4*.005;print $1"\t\t"$2"\t\t"$3"\t\t"$4"\t\t"x"\t\t"y"\t\t"z}' ./data/currentDisplacement.dat > ./data/currentDisplacementInches.dat	
	#gnuplot scripts/fdisplot.gp
	#gnuplot scripts/fplot.gp
	#gnuplot scripts/displot.gp
	
	#MOVE DATA TO SERVER	
	#cp /home/mfors1/Desktop/PCCOrg/currentFrequency.dat /home/mfors1/server/data/Current\ Test/
	#cp /home/mfors1/Desktop/PCCOrg/currentDisplacement.dat /home/mfors1/server/data/Current\ Test/
	#cp /home/mfors1/Desktop/PCCOrg/freqCurrent.jpeg /home/mfors1/server/data/Current\ Test/
	#cp /home/mfors1/Desktop/PCCOrg/freqdisCurrent.jpeg /home/mfors1/server/data/Current\ Test/
	#cp /home/mfors1/Desktop/PCCOrg/disCurrent.jpeg /home/mfors1/server/data/Current\ Test/
	

	echo "Next test in .5 hours" >> ./logs/tune$start.log
	
	#MOVE LOG TO SERVER
	#cp ./logs/tune$start.log /home/mfors1/server/data/Current\ Test/
	
	
	echo "Resting"
	sleep 30m
	


done
echo "Done Test"

#mv stretchData.txt ./data/$append/stretchData.txt
#mv testconfig.txt ./data/$append/testconfig.txt
