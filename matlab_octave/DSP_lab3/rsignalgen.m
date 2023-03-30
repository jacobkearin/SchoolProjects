function [r1, r2] = rsignalgen(x)

    t1 = sqrt((x.^2)+10000)/333.33333;
    t2 = sqrt(((x-0.4).^2)+10000)/333.33333;
    
    r1 = 1000*sin(2*pi*400-t1);
    r2 = 1000*sin(2*pi*400-t2);

    %for vector of values, use:  [r1, r2] = rsignalgen(-400:1:500);
    %for individual values, use: [r1, r2] = rsignalgen(-100);

end