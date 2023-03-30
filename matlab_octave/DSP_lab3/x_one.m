function [x1] = x_one(t, t1)
    x1 = 1000*sin(2*pi*400*(t - t1));
end