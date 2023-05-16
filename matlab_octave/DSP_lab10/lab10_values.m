clear;
load speechbad.mat;

ww = -2*pi:pi/100:2*pi;

f1 = 2222/fs;
f2 = 1555/fs;
fx1 = -2*cos(2*pi*f1);
fx2 = -2*cos(2*pi*f2);
bb1 = [1, fx1, 1];
bb2 = [1, fx2, 1];

yy = firfilt(bb1, xxbad);
yy = firfilt(bb2, yy);

H = freqz(bb1, 1, ww);
figure(1)
plot(ww, H);

soundsc(xxbad, fs); %play and plot original
figure(2)
plotspec(xxbad, fs);

soundsc(yy, fs, [-1, 1]); %play and plot filtered signal
figure(3)
plotspec(yy, fs);



