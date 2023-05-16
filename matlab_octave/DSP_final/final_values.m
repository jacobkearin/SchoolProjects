clear;
load datanotes.mat;

[m2, mf2] = toneDetectFFT(data2, fs);
[m3, mf3] = toneDetectFFT(data3, fs);
[m4, mf4] = toneDetectFFT(data4, fs);
[m5, mf5] = toneDetectFFT(data5, fs);
[m6, mf6] = toneDetectFFT(data6, fs);
[m7, mf7] = toneDetectFFT(data7, fs);
[m8, mf8] = toneDetectFFT(data8, fs);
[m9, mf9] = toneDetectFFT(data9, fs);



%soundsc(data2, fs);
subplot(4,2,1);
plotspec(data2, fs);
title(noteLetter(m2));

%soundsc(data3, fs);
subplot(4,2,2);
plotspec(data3, fs);
title(noteLetter(m3));

%soundsc(data4, fs);
subplot(4,2,3);
plotspec(data4, fs);
title(noteLetter(m4));

%soundsc(data5, fs);
subplot(4,2,4);
plotspec(data5, fs);
title(noteLetter(m5));

%soundsc(data6, fs);
subplot(4,2,5);
plotspec(data6, fs);
title(noteLetter(m6));

%soundsc(data7, fs);
subplot(4,2,6);
plotspec(data7, fs);
title(noteLetter(m7));

%soundsc(data8, fs);
subplot(4,2,7);
plotspec(data8, fs);
title(noteLetter(m8));

%soundsc(data9, fs);
subplot(4,2,8);
plotspec(data9, fs);
title(noteLetter(m9));


