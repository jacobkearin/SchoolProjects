bpm = 100;
bps = bpm/60;
spb = 1/bps;
spp = spb/4;
fs = 11025;

xx = zeros(1, ceil(240*spp*fs));
yy = xx;
for jj = 1:3
  scale.keys = theVoices(jj).noteNumbers;
  scale.durations = theVoices(jj).durations*spp;
  xx = zeros(1, ceil(240*spp*fs));
  for kk = 1:length(scale.keys)
    keynum = scale.keys(kk);
    tone = key2note(2, keynum, scale.durations(kk));
    n1 = floor(theVoices(jj).startPulses(kk)*fs*spp);
    n2 = n1+length(tone)-1;
    xx(n1:n2) = xx(n1:n2) + tone;
  end
  yy = yy + xx;
end

soundsc(yy, fs);

