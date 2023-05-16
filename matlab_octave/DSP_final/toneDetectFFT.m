function [maxNote, FC] = toneDetectFFT(dataIn, fs)

f0 = 440;
max1 = 0;
maxNote = -1;
data = fft(dataIn, fs);
FC = 0;
for note = 0:12;
  fc = f0 * 2.^(note/12);
  fh = fc + 5;
  fl = fc - 5;
  dataTemp = data;
  dataTemp(1:fl) = 0;
  dataTemp(fh:end) = 0;

  pow = max(abs(dataTemp));

  if pow > max1
    max1 = pow;
    maxNote = note;
    FC = fc;
  end

end
