function[theta] = direction(r1, r2)
    
    t1 = -asin(r1/1000);
    t2 = -asin(r2/1000);
    
    theta = asin((333.333*(t1-t2))/0.4);

    %after r1, r2 value are obtained in rsignalgen.m, theta value can be
    %obtained via: 
    %[theta] = direction(r1, r2);
    %output may be a vector or single value
    
end