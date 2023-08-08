load costssales.dat;
quarters = length(costssales);
fprintf('There are %d quarters in the file',quarters)
figure
costs = costssales(1:end/2);
sales = costssales(end/2+1:end);
figure
plot(1:length(costssales),costs,'ko',1:length(costssales),sales,'k*')
newdata = rot90(costssales);
save newfile.dat newdata -ascii