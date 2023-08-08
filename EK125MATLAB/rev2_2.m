subplot(1,2,1)
plot(0:.1:4,concencalc(85,(0:.1:4),120))
x = 120
subplot(1,2,2)
plot(0:.1:4,concencalc(85,(0:.1:4),300))
title('300 pounds')
function C = concencalc(C0,t,m)
    m = m * .4536;
    C = C0 * exp((-30*t)/m);
end
