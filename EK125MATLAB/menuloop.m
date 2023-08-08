choice = 0;
while choice == 0
    choice = menu('choose','fix','floor','ceil');
end
num = rand * 10;
disp(num)
switch choice
    case 1 
        num = fix(num);
    case 2
        num = floor(num);
    case 3
        num = ceil(num);
end
disp(num)
        
        
        
    