rawData=dlmread('temp.log');
rawData=rawData(:,1:16);
rawData=reshape(rawData', 128*16, 1);
tempDegC=((rawData* power(2,3)) - 200)/10;
tempDegF=((tempDegC * 9) / 5) + 32;
plot(tempDegF(1:200))
grid