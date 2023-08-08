vec = randi([0,100],1,10);
vec = sort(vec);
p = polyfit(1:10,vec,1);
hold
plot(1:10,polyval(p,1:10))
plot(1:10,vec,'*')
legend('Straight line','Points')