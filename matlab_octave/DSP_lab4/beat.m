function [xx, tt] = beat(a, b, fc, delf, fsamp, dur)
    tt = 0:1/fsamp:dur;
    fa = fc-delf;
    fb = fc+delf;
    xx = a*cos(2*pi*fa*tt)+b*cos(2*pi*fb*tt);
end