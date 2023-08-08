negnums = [0 0 0 0 0 0]
n = randi([3 6]);
num = 0;
for i = 1:n
    entry = input('enter a number: ');
    if entry < 0
        num = num + 1;
        negnums(num) = entry;
    end
end
negnums