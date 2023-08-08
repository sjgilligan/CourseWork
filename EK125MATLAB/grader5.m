mat = randi([1,10],randi([2,6]),randi([3,5]));
[r c] = size(mat);
for i = 1:r
    subplot(1,r,i);
    x = 1:c;
    y = mat(i,1:c);
    plot(x,y);
end
    




