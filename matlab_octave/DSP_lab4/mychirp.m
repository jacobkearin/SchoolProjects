function [xx,tt] = mychirp( f1, f2, dur, fsamp)

    if(nargin < 4), fsamp = 11025; end
    tt = 0:(1/fsamp):dur;
    s = (f2-f1)/(2*dur);
    psi = 2*pi*(s*tt.*tt + f1*tt);
    xx = real( 7.7*exp(j*psi));

end
