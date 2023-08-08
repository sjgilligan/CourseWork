x = 1:10;
y = [2, 2.7, 3.8, 4, 4.5, 5.3, 6.3, 6.4, 7.1, 9];
corr = mean(mean(corrcoef(x,y)));
mytitle = sprintf('Correlation coefficient %.3f',corr);
plot(x,y,'*')
title(mytitle)
xlabel('x')
ylabel('y')