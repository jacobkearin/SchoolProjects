dur = 0.5;
amp = 2;
keynum = 48;

out = key2note(amp, keynum, dur);

soundsc(out, 11025);

