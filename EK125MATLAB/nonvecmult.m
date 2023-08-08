mat = randi([0 10],5,5)
for j = 1:height(mat)
    for i = 1:length(mat)
        mat(j,i) = mat(j,i) + 5;
    end
end
disp(mat)