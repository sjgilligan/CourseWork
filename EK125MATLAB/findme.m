min = input('Enter a minimum value: ');
max = input('Enter a maximum value: ');
x = input('Enter your value: ');
count = 0;
random = NaN;
while x ~= random
    random = randi([min max]);
    count = count+1;
end
fprintf('it took %d tries to generate your number',count)

    
