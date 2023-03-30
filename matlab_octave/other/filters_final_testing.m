% elliptic filter, sinc filter and raised cosine filter testing
% Jacob Kearin
clear;

t = [-20:1:20];
tf = [-20:.1:20];
tlen = length(tf)/2;
tfh = [1:tlen];

imp = sinc(tf);
fulldsig = zeros(1, 401);
dsig = randint(1, 40, 2);
for i = 1:40
  fulldsig((10*(i-1))+1:(10*i)) = dsig(i);
end

%figure(5)
%plot(fulldsig)



inres = 2; %resolution of input values (1 bit = 2 values)

sig = randint(1, length(t), inres);
fsig = upsample(sig, 10); % full signal
fsig([:,402:end]) = []; % full signal trimming
sincout = conv(imp, fsig, "same"); % convolution of digital signal and the sinc filter impulse

hrcos = rcosfir(0.25,[-20 20],10,1, "normal"); % raised cosine filter impulse function

rcosout = conv(hrcos, fsig, "full"); % convolution of digital signal and the rcos filter impulse
%rcosout = conv(hrcos, fulldsig, "full")/10 % convolution of digital signal and the rcos filter impulse
rcosout([:,1:200, 602:end]) = []; % re-aligning the output and trimming tail

%elliptic filter creation
ord   = 3;    % filter order
Rp    = 0.2;  % ripple factor in the passband (dB)
Rs    = 20;   % attenuation in the stopband (dB)
fs    = 1000;  % sampling rate (hz)
fmax  = fs/2; % Nyquist frequency
fl    = 1;    % low end of passband
fh    = 333;   % high end of passband
[b,a] = ellip(ord, Rp, Rs, [fl/fmax, fh/fmax]);

ellipout = filter(b, a, fsig);


ftc = fft(rcosout); %fft of rcos filtered message
ft0 = fft(fsig); %fft of original
fts = fft(sincout); %fft of sinc filtered message
fte = fft(ellipout); %fft of elliptic filtered message

ft0([:,201:end]) = []; % full signal trimming
ftc([:,201:end]) = []; % full signal trimming
fts([:,201:end]) = []; % full signal trimming
fte([:,201:end]) = []; % full signal trimming




figure(1) % ideal sinc and raised cosine impulse functions
plot(imp);
hold on
plot(hrcos);
axis([0 401 -0.5 1.1])
title "sinc and rcos inpulse function";



figure(2)
subplot(321) %sinc filter output and message input
%plot(tf, sincout);
plot(-21,0);
hold on
plot(tf, fsig);
axis([-20 20 -0.5 (inres-.5)])
title "original, unfiltered input";


subplot(323) %rcos filter ouput and message input
plot(rcosout);
hold on
plot(fsig);
axis([0 401 -0.5 (inres-.5)])
title "raised cosine filter";


subplot(325) %elliptic filter output and message output
plot(ellipout);
hold on
plot(fsig);
axis([0 401 -0.5 (inres-.5)])
title "elliptic filter";


subplot(322) % fft of original message
plot(tfh, ft0);
title "fft of original message"


subplot(324) % fft of rcos filtered message
plot(tfh, ftc);
title "fft of rcos filtered output"


subplot(326) % fft of elliptic filtered message
plot(tfh, fte);
title "fft of elliptic filtered output"


%uncomment below for rcos eye diagram

%{
figure(3)
plot(rcosout, "b");
hold on
axis([0 101 -0.5 (inres-.5)]);
%}




