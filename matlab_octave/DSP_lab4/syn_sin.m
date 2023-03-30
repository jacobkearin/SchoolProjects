function [xx, tt] = syn_sin(fk, Xk, tshift, fs, dur, tstart)
    if nargin<5, tstart = 0; end
    len = length(fk);
    tt = tstart:1/fs:tstart+dur ;
    xx = zeros(1, length(tt)) ;
    for index = 1:len
        y = Xk(index) * exp(1i*2*pi*fk(index)*(tt+tshift(index)));
        xx = real(xx+y);
    end
    %plot(tt, xx);
end


%added tshift for individual sinusoidal phase shifts


