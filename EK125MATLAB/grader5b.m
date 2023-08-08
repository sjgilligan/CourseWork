mat = randi([-100,100],randi([15,20]),randi([15,20]));
matsum = zeros(1,height(mat))
for j = 1:length(mat)
    mysum = 0;
    mysum = mysum + mat(h,j);
    matsum(j) = mysum
end
sum(mat')
    
    