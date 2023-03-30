x = 100;
t1 = t_one(x);
t2 = t_two(x);
t = 0:0.000001:.01;
x1 = x_one(t, t1);
x2 = x_two(t, t2);

hold on
plot(t, x1);
plot(t, x2);
title('signal received vs time')
xlabel('Time (sec)')
ylabel('complex amplitude')
