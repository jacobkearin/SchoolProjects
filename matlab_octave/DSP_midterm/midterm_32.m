scale.keys = [ 40 42 44 45 47 49 51 52 ];
%--- NOTES: C D E F G A B C
% key #40 is middle-C
scale.durations = 0.5 * ones(1,length(scale.keys));
fs = 11025; %-- or 8000 Hz
xx = zeros(1, sum(scale.durations)*fs+length(scale.keys) );
n1 = 1;
for kk = 1:length(scale.keys)
  keynum = scale.keys(kk);
  tone = key2note(2, keynum, scale.durations);
  n2 = n1 + length(tone) - 1;
  xx(n1:n2) = xx(n1:n2) + tone; %<=== Insert the note
  n1 = n2 + 1;
end
soundsc( xx, fs )

specgram(xx,512,11025);

