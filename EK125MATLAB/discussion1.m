mat2 = [2 6 7; 3 4 1; 8 9 5];
[r c] = size(mat2);
vec = zeros(1,r);
for row = 1:r
    mysum = 0;
    for col = 1:c
        mysum = mysum + mat2(row, col);
    end
    vec(1,row) = mysum;
end
vec
sum(mat2')