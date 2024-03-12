mkdir -p ./out
g++ --std=c++17  -I../common  ./main.cpp   -o ./out/draw.exe
cd ./out
./draw.exe
rm ./draw.exe
ppmtojpeg ./xxxx0.ppm > xxx0.jpg 
ppmtojpeg ./xxxx1.ppm > xxx1.jpg 
ppmtojpeg ./xxxx2.ppm > xxx2.jpg 
ppmtojpeg ./xxxx3.ppm > xxx3.jpg 
ppmtojpeg ./xxxx4.ppm > xxx4.jpg 
rm ./xxxx0.ppm
rm ./xxxx1.ppm
rm ./xxxx2.ppm
rm ./xxxx3.ppm
rm ./xxxx4.ppm
cd ..