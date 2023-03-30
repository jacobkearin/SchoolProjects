% Testing and simulations for multicarrier QAM report
% Jacob Kearin

clear;


% basic QAM-16 constellation mapping
gen_d = randint (1, 1e4, 16);
gen1_c = [3+3j 1+3j -1+3j -3+3j 3+j 1+j -1+j -3+j 3-j 1-j -1-j -3-j 3-3j 1-3j -1-3j -3-3j];
gen1_y = genqammod (gen_d, gen1_c);
gen1_z = awgn (gen1_y, 9);
gen1_dm = genqamdemod(gen1_z, gen1_c);

gen2_y = pskmod(gen_d, 16, 0, "gray")*3*sqrt(2);
gen2_z = awgn (gen2_y, 9);

pg = [3];

figure(1)
subplot(121)
hold on
plot(gen1_z, "r.")
plot(0,0, "k+");
quiver(0, 0, pg, pg, "k");
axis([-5.5 5.5 -5.5 5.5]);
title "16QAM"

subplot(122)
hold on
plot(gen2_z, "r.")
plot(0,0, "k+");
quiver(0, 0, pg, pg, "k");
axis([-5.5 5.5 -5.5 5.5]);
title "16PSK"


t = 0:0.01:100; %simulation time
mwid = 50;      %symbol length in signal
mlen = 10001;   %length of simulated signal
symsize = 4;    %symbol bitsize^2:  2 bits ^2 = size 4 = qam16
mlw = (mlen-1)/mwid;

mti = zeros(1, mlen);
mtq = zeros(1, mlen);
mtirand = (randint(1, mwid, symsize)*2)-(symsize-1);
mtqrand = (randint(1, mwid, symsize)*2)-(symsize-1);
for i = 1:mwid
  mti((mlw*(i-1))+1:(mlw*i)) = mtirand(i);
  mtq((mlw*(i-1))+1:(mlw*i)) = mtqrand(i);
end

mtiu = upsample(mtirand, 200);
mtqu = upsample(mtqrand, 200);

hrcos = rcosfir(0.1,[-20 20],10,1, "normal"); % raised cosine filter impulse function
%filter the messages

[butb, buta] = butter(8, 1/2);

frcmti = conv(hrcos, mti, "full")/10;
frcmti([:,1:199, 10201:end]) = []; %signal trimming

frcmtq = conv(hrcos, mtq, "full")/10;
frcmtq([:,1:199, 10201:end]) = []; %signal trimming


fbutmti = filter(butb, buta, mti);
fbutmtq = filter(butb, buta, mtq);


qcar = sin(10*t);
icar = cos(10*t);

iwave = icar.*mti;
qwave = qcar.*mtq;


qamout = iwave+qwave;

%rotqamout = qamout.*exp(j*200*t);
%rotqamout = genqamdemod(qamout, gen1_c).*exp(j*200*t);


%outdemod = qamdemod(qamout, 16)


figure(2)
subplot(311)
hold on
plot(t, (iwave/2)-2, "b");
plot(t, (mti/2)+2, "k");
%plot(t, frcmti, "g");
%plot(t, fbutmti, "r");
title "In phase message and signal (I)"
axis([0 20 -4 4]);

subplot(312)
hold on
plot(t, (qwave/2)-2, "b");
plot(t, (mtq/2)+2, "k");
%plot(t, frcmtq, "g");
%plot(t, fbutmtq, "r");
title "Out of phase message and signal (Q)"
axis([0 20 -4 4]);

subplot(313)
hold on
plot(t, qamout, "b");
axis([0 20 -4.5 4.5]);
title "Output QAM signal"



%figure(3)
%plot(rotqamout);
%plot(outdemod);
%axis([0 1000 0 16]);




