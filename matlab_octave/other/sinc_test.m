
t = [-20:1:20];
tf = [-20:.1:20];

imp = sinc(tf);

sig = randint(1, length(t));
fsig = upsample(sig, 10);
fsig([:,402:end]) = [];
y = conv(imp, fsig, "same");



h = rcosfir(0.2,[-20 20],10,1, "normal") % raised cosine filter impulse function


yy = conv(h, fsig, "full") % convolution of digital signal and the rcos filter

yy([:,1:200]) = []; % accounting for time delay

figure(1)
plot(tf, y);

hold on
plot(tf, fsig);

figure(2)
plot(yy);
hold on
plot(fsig);

figure(3)
plot(imp)
hold on
plot(h)


