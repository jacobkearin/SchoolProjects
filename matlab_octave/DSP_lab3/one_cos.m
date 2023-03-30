function [yy] = one_cos(A, w, ph, dur)
    ff = w / (2*pi);
    t = 0:1/(32*ff):dur;
    yy = plot(t, A*cos(w*t+ph));
end

%one_cos(ocA, ocw, ocph, ocdur)